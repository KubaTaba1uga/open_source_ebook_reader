#!/usr/bin/env python3
"""Summarize stakeholder -> expectation -> objective -> use case links."""
import argparse
import re
from collections import defaultdict
from pathlib import Path

EXP = re.compile(r'EXP-[A-Za-z]+-\d+')
OBJ = re.compile(r'OBJ-[A-Za-z]+-\d+')
UC = re.compile(r'UC-\d+')


def parse_stakeholders(text):
    expectations = {}
    objectives = {}
    links = defaultdict(set)
    for line in text.splitlines():
        if line.startswith('|'):
            cells = [c.strip() for c in line.strip().strip('|').split('|')]
            if len(cells) >= 3 and EXP.fullmatch(cells[0]):
                expectations[cells[0]] = (cells[1], cells[2])
            elif len(cells) >= 2 and OBJ.fullmatch(cells[0]):
                for exp in EXP.findall(cells[1]):
                    links[exp].add(cells[0])
        m = re.match(r'\s*-\s*<a id="(OBJ-[A-Za-z]+-\d+)"></a>\s*`?\1`?\s*:?\s*(.*)', line)
        if m:
            objectives[m[1]] = m[2].strip().lstrip('`').strip()
    return expectations, objectives, links


def parse_usecases(text):
    cases = {}
    current = None
    for line in text.splitlines():
        if line.lstrip().startswith('<!--'):
            continue
        m = re.search(r'\*\*ID:?\*\*:\s*<a id="(UC-\d+)"></a>', line)
        if m:
            current = m[1]
            cases[current] = {'title': '', 'objectives': set(), 'unresolved': False}
            continue
        if current is None:
            continue
        m = re.match(r'###\s+(.+)', line)
        if m:
            current = None
            continue
        if '**Related objectives' in line:
            cases[current]['objectives'].update(OBJ.findall(line))
            if not cases[current]['objectives']:
                cases[current]['unresolved'] = True
    # Titles precede IDs, so extract each heading + ID pair.
    for title, uid in re.findall(r'###\s+([^\n]+)\s+\*\*ID:?\*\*:\s*<a id="(UC-\d+)"></a>', text):
        if uid in cases:
            cases[uid]['title'] = title.strip()
    return cases


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('stakeholder', help='e.g. User or STAKE-User')
    p.add_argument('--stakeholders', type=Path, default=Path('00_stakeholders.md'))
    p.add_argument('--conops', type=Path, default=Path('01_conops.md'))
    args = p.parse_args()
    role = args.stakeholder.removeprefix('STAKE-')
    expectations, objectives, links = parse_stakeholders(args.stakeholders.read_text())
    cases = parse_usecases(args.conops.read_text())
    selected = [(eid, data) for eid, data in expectations.items() if eid.startswith(f'EXP-{role}-')]
    if not selected:
        p.error(f'No expectations found for stakeholder {role!r}')
    print(f'STAKE-{role}')
    for eid, (description, priority) in selected:
        print(f'  {eid} [{priority}]: {description}')
        related = sorted(links[eid])
        if not related:
            print('    [GAP] No linked objectives')
        for oid in related:
            print(f'    {oid}: {objectives.get(oid, "[description missing]")}')
            matching = [(uid, c['title']) for uid, c in cases.items() if oid in c['objectives']]
            if not matching:
                print('      [GAP] No linked use cases')
            for uid, title in sorted(matching):
                print(f'      {uid}: {title}')
    unresolved = [uid for uid, c in cases.items() if c['unresolved']]
    if unresolved:
        print('\nUse cases without explicit objective IDs: ' + ', '.join(sorted(unresolved)))


if __name__ == '__main__':
    main()
