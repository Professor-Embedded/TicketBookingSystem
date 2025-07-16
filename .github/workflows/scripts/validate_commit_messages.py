import subprocess
import re
import sys

def run(cmd):
    result = subprocess.run(cmd, shell=True, text=True, capture_output=True)
    if result.returncode != 0:
        print(result.stderr)
        sys.exit(1)
    return result.stdout.strip()

def get_pr_commit_shas():
    # This fetches all commits between base and HEAD in the PR
    base_sha = run("git merge-base origin/main HEAD")
    return run(f"git rev-list {base_sha}..HEAD").splitlines()

def get_commit_message(sha):
    return run(f"git log -1 --pretty=format:%B {sha}")

def validate_commit_message(message, sha):
    lines = message.strip().split('\n')

    if len(lines) != 12:
        raise ValueError(f"❌ Commit {sha}: expected 12 lines, found {len(lines)}")

    validators = [
        (r'^[a-zA-Z0-9._-]+ [0-9]+\.[0-9]+: Fix CVE-[0-9]{4}-[0-9]{4,}$', "Line 1 format: '<Package> <Version>: Fix CVE-XXXX-YYYY'"),
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

    for i, (pattern, error_message) in enumerate(validators):
        if not re.match(pattern, lines[i]):
            raise ValueError(f"❌ Commit {sha}, Line {i+1} invalid: {error_message}\n   → {lines[i]}")

if __name__ == "__main__":
    shas = get_pr_commit_shas()
    for sha in shas:
        print(f"🔍 Checking commit {sha}")
        msg = get_commit_message(sha)
        print(msg + "\n" + "-" * 40)
        validate_commit_message(msg, sha)

    print("✅ All commit messages passed validation.")
