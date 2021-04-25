import os
import argparse

parser = argparse.ArgumentParser(
    description="Resubmitting jobs with different statuses."
)

statuses = [
    "ERROR_V",
    "ERROR_E",
    "ERROR_IB",
    "ERROR_SV",
    "ERROR_RE",
    "EXPIRED",
    "ZOMBIE",
    "ERROR_SPLT",
]
parser.add_argument(
    "-i",
    metavar="--error-index",
    type=int,
    nargs=1,
    help=f"Specify index of the error status from the list {statuses}.",
)

i = parser.parse_args().i[0]


masterJobs = []

with open("masterjobs", "r") as m:
    for mj_id in m.readlines():
        masterJobs.append(int(mj_id))

if not masterJobs:
    raise LookupError("Nothing in masterjob file or can't parse it.")

subjobs = []

status = statuses[i]

for job in masterJobs:
    if status != "ERROR_SPLT":
        subjobs.append(
            os.popen(f"alien.py masterjob {job} -status {status} -printid").readlines()
        )
    else:
        os.system(f"alien.py resubmit {job}")

for subjob in subjobs:
    os.system(f'alien.py resubmit {subjob[3][44:-2].replace(",", " ")}')
