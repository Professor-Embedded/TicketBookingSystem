import subprocess
import sys
import json
import re

def run(cmd):
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, text=True)
    if result.returncode != 0:
        print(result.stderr)
        sys.exit(1)
    return result.stdout.strip()

def get_commit_shas(pr_number):
    cmd = f'gh pr view {pr_number} --json commits --jq ".commits[].oid"'
    return run(cmd).splitlines()

def get_commit_message(sha):
    return run(f'git log -1 --pretty=format:%B {sha}')

def validate_commit_message(message, sha):
    lines = message.strip().split('\n')
    if len(lines) != 12:
        raise ValueError(f"❌ Commit {sha} must have exactly 12 lines (found {len(lines)})")

    validators = [
        (r'^[a-zA-Z0-9._-]+ [0-9]+\.[0-9]+: Fix CVE-[0-9]{4}-[0-9]{4,}$', "Line 1 format: '<Package Name> <Package Version>: Fix CVE-XXXX-YYYY'"),
        (r'^$', "Line 2 must be blank"),
        (r'^Upstream Repository: https?://\S+$', "Line 3 format: 'Upstream Repository: <URL>'"),
        (r'^$', "Line 4 must be blank"),
        (r'^Bug Details: https?://\S+$', "Line 5 format: 'Bug Details: <URL>'"),
        (r'^Type: Security Fix$', "Line 6 must be 'Type: Security Fix'"),
        (r'^CVE: CVE-[0-9]{4}-[0-9]{4,}$', "Line 7 format: 'CVE: CVE-XXXX-YYYY'"),
        (r'^Score: [0-9]+(\.[0-9]+)?$', "Line 8 format: 'Score: <CVSS>'"),
        (r'^Patch: https?://\S+$', "Line 9 format: 'Patch: <URL>'"),
        (r'^$', "Line 10 must be blank"),
        (r'^Jira-Id: [A-Z]+-[0-9]+$', "Line 11 format: 'Jira-Id: PROJECT-ID'"),
        (r'^Signed-off-by: .+ <.+@.+>$', "Line 12 format: 'Signed-off-by: Name <email>'")
    ]

    for i, (pattern, err_msg) in enumerate(validators):
        if not re.match(pattern, lines[i]):
            raise ValueError(f"❌ Commit {sha}, Line {i+1} is invalid. {err_msg}\nLine content: '{lines[i]}'")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("❌ PR number required as argument")
        sys.exit(1)

    pr_number = sys.argv[1]
    print(f"🔍 Validating commit messages for PR #{pr_number}...\n")

    try:
        shas = get_commit_shas(pr_number)
        for sha in shas:
            msg = get_commit_message(sha)
            print(f"✔️ Checking commit {sha}...\n{msg}\n")
            validate_commit_message(msg, sha)
        print("\n✅ All commit messages are valid.")
    except Exception as e:
        print(str(e))
        sys.exit(1)
