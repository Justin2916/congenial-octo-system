#!/usr/bin/env python3
"""Port check: runs tools/lockstep.py for every script in port/scripts against every test save, comparing a reference
ROM (the build from before the change) with the current CritterKeep.gba. Prints one line each; exit 1 on any difference.
  python3 tools/port_check.py REF.gba [scripts...]
"""
import os, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ref = os.path.abspath(sys.argv[1])
import shutil, tempfile
CUR = os.path.join(tempfile.mkdtemp(prefix='pc'), 'cur.gba'); shutil.copy(os.path.join(ROOT, 'CritterKeep.gba'), CUR)   # (a rebuild meanwhile can't mix in)
scripts = sys.argv[2:] or sorted(os.path.join(ROOT, 'port', 'scripts', f) for f in os.listdir(os.path.join(ROOT, 'port', 'scripts')))
saves = [os.path.join(ROOT, 'tests', 'saves', s + '.sav') for s in ('adult', 'hatched', 'teen', 'oldgrown', 'oldchild')]
def one(job):
    sc, sv = job
    r = subprocess.run([sys.executable, os.path.join(ROOT, 'tools', 'lockstep.py'), ref, CUR, sc, sv],
                       capture_output=True, text=True)
    return f'{os.path.basename(sc):12s} {os.path.basename(sv):13s} ' + (r.stdout.strip() or r.stderr.strip()[-300:]), r.returncode
bad = 0
with ThreadPoolExecutor(os.cpu_count() or 4) as ex:
    for line, rc in ex.map(one, [(a, b) for a in scripts for b in saves]):
        print(line if rc else line.splitlines()[0]); bad += rc != 0
print('ALL IDENTICAL' if not bad else f'{bad} DIFFER'); sys.exit(1 if bad else 0)
