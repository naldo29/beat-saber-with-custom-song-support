"""Runtime-table generation is independent of current per-build generic target."""
import os,pathlib,subprocess,json
root=pathlib.Path(__file__).resolve().parents[1]
outputs=[]
for name in ['beatsaber140','x9999999999']:
 outputs.append(subprocess.check_output(['python3','visionos/targets.py','--c-table'],cwd=root,env=dict(os.environ,KLEPTON_TARGET=name),text=True))
assert outputs[0]==outputs[1]==(root/'runtime/kl_target_table.h').read_text()
for name in json.loads((root/'visionos/runtime-targets.json').read_text())['targets']:
 assert 'KL_TARGET_ROW("'+name+'"' in outputs[0]
print('PASS deterministic runtime-table generation retains all legacy generic game rows')
