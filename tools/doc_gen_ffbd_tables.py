
#!/usr/bin/env python3

import argparse
import re
from pathlib import Path

MOE_PATTERN = re.compile(
    r"`(MOE-\d+)`:\s*(.+)"
)

FFBD_PATTERN = re.compile(
    r"(?=^##\s+)",
    re.MULTILINE,
)

FFBD_ID_PATTERN = re.compile(
    r'^\*\*ID\*\*:\s*<a id="(FFBD-\d+)"></a>\s*\1\s*$',
    re.MULTILINE,
)

LINK_PATTERN = re.compile(
    r"\[(MOE-\d+)\]\(([^)]+)\)"
)

RELATED_PATTERN = re.compile(
    r"^\*\*Related MOEs:\*\*[^\n]*$",
    re.MULTILINE,
)


def load_moes(path):
    moes = {}

    for line in path.read_text(encoding="utf-8").splitlines():
        match = MOE_PATTERN.search(line)

        if match:
            moe_id, description = match.groups()

            if moe_id in moes:
                raise ValueError(f"Duplicate MOE: {moe_id}")

            moes[moe_id] = description.strip()

    return moes


def generate_table(section, moes):
    ffbd = FFBD_ID_PATTERN.search(section)

    if not ffbd:
        return section

    related = RELATED_PATTERN.search(section)

    if not related:
        raise ValueError(
            f"Missing Related MOEs field in {ffbd.group(1)}"
        )

    links = LINK_PATTERN.findall(related.group(0))

    if not links:
        raise ValueError(
            f"No MOEs found in {ffbd.group(1)}"
        )

    rows = [
        "| MOE | Success criterion |",
        "|---|---|",
    ]

    seen = set()

    for moe_id, url in links:
        if moe_id in seen:
            continue

        seen.add(moe_id)

        if moe_id not in moes:
            raise ValueError(
                f"Unknown MOE {moe_id} in {ffbd.group(1)}"
            )

        description = moes[moe_id].replace("|", r"\|")

        rows.append(
            f"| [{moe_id}]({url}) | {description} |"
        )

    table = "\n".join(rows)

    return RELATED_PATTERN.sub(
        lambda match: table,
        section,
        count=1,
    )


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("success_criteria", type=Path)
    parser.add_argument("logical_decomposition", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    moes = load_moes(args.success_criteria)

    content = args.logical_decomposition.read_text(
        encoding="utf-8"
    )

    sections = FFBD_PATTERN.split(content)

    result = "".join(
        generate_table(section, moes)
        for section in sections
    )

    args.output.parent.mkdir(
        parents=True,
        exist_ok=True,
    )

    if (
        not args.output.exists()
        or args.output.read_text(encoding="utf-8") != result
    ):
        args.output.write_text(
            result,
            encoding="utf-8",
        )


if __name__ == "__main__":
    main()
