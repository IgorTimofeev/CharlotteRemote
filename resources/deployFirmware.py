from datetime import datetime
import subprocess
from pathlib import Path
import shutil

currentPath = Path().absolute()

firmwareName = 'Remote.bin'
firmwarePath = currentPath / 'build' / firmwareName

downloadsPath = currentPath.parent / 'Downloads'
downloadsFirmwarePath = downloadsPath / 'Firmware' / firmwareName
downloadsBranch = 'master'
downloadsCommitMessage = f'Auto-deployed via blazingly fast zaebashit.py CI/CD tool at {datetime.utcnow():%d.%m.%Y %H:%M:%S)}'

# Copying firmware file
print(f'Copying firmware from {firmwarePath} to {downloadsFirmwarePath}')

shutil.copy2(firmwarePath, downloadsFirmwarePath)

# Pushing to downloads repo
print(f'Pushing to downloads repository in {downloadsBranch} branch')

subprocess.run(f'git -C {downloadsPath} checkout {downloadsBranch}', shell = True)
subprocess.run(f'git -C {downloadsPath} add .', shell = True)
subprocess.run(f'git -C {downloadsPath} commit -m "{downloadsCommitMessage}"', shell = True)
subprocess.run(f'git -C {downloadsPath} push -u origin {downloadsBranch}"', shell = True)