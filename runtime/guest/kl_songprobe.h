#ifndef KL_SONGPROBE_H
#define KL_SONGPROBE_H

/* Opt-in, read-only capability probe. This does not load custom songs or QMODs.
 * Run between Unity frames on the managed thread, as for kl_songs_tick. */
void kl_songprobe_tick(const char *target, unsigned frame);

#endif
