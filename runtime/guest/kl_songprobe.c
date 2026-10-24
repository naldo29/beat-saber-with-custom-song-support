/* First bring-up step for a hook-free Beat Saber 1.40 custom-song loader.
 * Compile into the signed runtime. No writable code, trampolines, directory
 * scanning, or untrusted plugin loading. Enable with KL_SONG_PROBE=1.
 * A successful managed call proves only the bridge, not song playback. */
#include <stdio.h>
#include <string.h>
#include "klepton.h"
#include "kl_songprobe.h"

#define LOG(...) fprintf(stderr, "  [song-probe] " __VA_ARGS__)

typedef struct {
    void *(*domain_get)(void);
    const void **(*domain_get_assemblies)(void *, size_t *);
    void *(*assembly_get_image)(const void *);
    void *(*class_from_name)(void *, const char *, const char *);
    const void *(*class_get_method_from_name)(void *, const char *, int);
    const void *(*class_get_methods)(void *, void **);
    const char *(*method_get_name)(const void *);
    unsigned (*method_get_param_count)(const void *);
    void *(*string_new)(const char *);
    void *(*runtime_invoke)(const void *, void *, void **, void **);
} probe_api;

static probe_api api;
static int finished, enabled = -1;

static int bind_api(kl_image *image) {
#define BIND(field) do { \
    *(void **)&api.field = kl_sym(image, "il2cpp_" #field); \
    if (!api.field) { LOG("STOP missing export il2cpp_%s\n", #field); return 0; } \
} while (0)
    BIND(domain_get);
    BIND(domain_get_assemblies);
    BIND(assembly_get_image);
    BIND(class_from_name);
    BIND(class_get_method_from_name);
    BIND(class_get_methods);
    BIND(method_get_name);
    BIND(method_get_param_count);
    BIND(string_new);
    BIND(runtime_invoke);
#undef BIND
    return 1;
}

static void *find_class(const void **assemblies, size_t count,
                        const char *ns, const char *name) {
    for (size_t i = 0; i < count; i++) {
        void *image = api.assembly_get_image(assemblies[i]);
        void *klass = image ? api.class_from_name(image, ns, name) : NULL;
        if (klass) return klass;
    }
    return NULL;
}

static void report_class(const void **assemblies, size_t count,
                          const char *ns, const char *name) {
    void *klass = find_class(assemblies, count, ns, name);
    LOG("class %s%s%s %s\n", ns, *ns ? "." : "", name,
        klass ? "PRESENT" : "ABSENT");
    if (!klass) return;
    void *iter = NULL;
    const void *method;
    unsigned n = 0;
    while ((method = api.class_get_methods(klass, &iter)) != NULL) {
        if (++n > 128) { LOG("  method list truncated at 128\n"); break; }
        const char *mn = api.method_get_name(method);
        LOG("  %s/%u\n", mn ? mn : "(unnamed)", api.method_get_param_count(method));
    }
}

void kl_songprobe_tick(const char *target, unsigned frame) {
    if (finished || !target || strcmp(target, "beatsaber140")) return;
    if (enabled < 0) {
        enabled = kl_env_on("KL_SONG_PROBE", 0);
        if (enabled) LOG("WAIT starting bridge/capability probe (no song playback)\n");
    }
    if (!enabled || frame < 90 || frame % 30) return;
    if (frame >= 7200) {
        LOG("STOP managed domain/classes not ready before frame limit\n");
        finished = 1;
        return;
    }
    kl_image *image = kl_find_image("libil2cpp.so");
    if (!image) return;  /* Unity may not have loaded it yet. */
    if (!api.domain_get && !bind_api(image)) { finished = 1; return; }
    void *domain = api.domain_get();
    if (!domain) return;
    size_t count = 0;
    const void **assemblies = api.domain_get_assemblies(domain, &count);
    if (!assemblies || !count) return;
    void *debug = find_class(assemblies, count, "UnityEngine", "Debug");
    void *model = find_class(assemblies, count, "", "BeatmapLevelsModel");
    if (!debug || !model) return;

    finished = 1;  /* A managed error is final; never spam an exception each frame. */
    const void *log = api.class_get_method_from_name(debug, "Log", 1);
    void *message = api.string_new("[song-probe] managed bridge reached; custom song playback not implemented");
    if (!log || !message) { LOG("STOP Debug.Log/1 or message unavailable\n"); return; }
    void *args[] = { message }, *exception = NULL;
    /* A void method returns NULL on success. Only the exception slot is an error. */
    api.runtime_invoke(log, NULL, args, &exception);
    if (exception) { LOG("STOP Debug.Log threw a managed exception\n"); return; }
    LOG("BRIDGE_OK Debug.Log invoked; assemblies=%zu\n", count);
    static const char *const classes[] = {
        "BeatmapLevelsModel", "CustomLevelLoader", "BeatmapLevel",
        "FileSystemBeatmapLevelData", "BeatmapLevelData", "BeatmapDataLoader",
        "CustomPreviewBeatmapLevel", "CustomBeatmapLevelPack",
        "CustomBeatmapLevelCollection", "BeatmapLevelPack",
        "BeatmapLevelPackCollection", "StandardLevelInfoSaveData",
        "LevelFilteringNavigationController", "CachedMediaAsyncLoader",
        "AudioClipAsyncLoader"
    };
    for (size_t i = 0; i < sizeof classes / sizeof classes[0]; i++)
        report_class(assemblies, count, "", classes[i]);
    report_class(assemblies, count, "UnityEngine.Networking", "UnityWebRequestMultimedia");
    LOG("DONE capability inventory only; custom song playback NOT implemented\n");
    fflush(stderr);
}
