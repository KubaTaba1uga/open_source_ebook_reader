Concept of Operations (ConOps)
==========================================

This document describes how the Open Source E-book Reader is intended to operate from the perspective of its stakeholders. 


## 1. Purpose

The Concept of Operations (ConOps) translates stakeholder needs, goals, and objectives into descriptions of expected system behavior and interactions. It provides a common understanding of what the system should accomplish, how it will be used, and under what conditions it must operate.

The ConOps serves as a foundation for developing technical requirements, defining system architecture, and planning verification and validation activities.

## 2. Use cases

Each use case is assigned to at leas one Objective which content can find in Stakeholders document using it's ID (OBJ-xxx-nn).

The structure of use case is pretty simple, each use case contain multiple fields, each describing particular feature of the use case:

| Field              | Purpose                                               |
| ------------------ | ----------------------------------------------------- |
| ID                 | Unique identifier                                     |
| Related objectives | Links to stakeholder objectives                       |
| Preconditions      | What must be true before the scenario starts          |
| Trigger            | What starts the scenario                              |
| Main flow          | Normal sequence of actor actions and system responses |
| Alternative flows  | Errors, exceptional conditions, or alternative paths  |
| Postconditions     | What should be true when the scenario finishes        |

### Power On the device

**ID**: <a id="UC-001"></a>UC-001

**Related objectives**: [OBJ-User-01](/design/00_stakeholders.md#OBJ-User-01)

**Preconditions**: Device has enough power to operate. Device is powered off.

**Postconditions**: The device either reaches a ready-to-use state or reports a startup failure and powers off.

**Trigger**: User presses the power button.

**Main flow**
![Main Flow](/design/assets/UC-001.png)


### Select and open a book

**ID**: <a id="UC-002"></a>UC-002

**Related objectives**: [OBJ-User-01](/design/00_stakeholders.md#OBJ-User-01)

**Preconditions**: Device is fully operational. Device is powered on.

**Postconditions**: Book's page is displayed on the screen.

**Trigger**: Ebook reader app has started.

**Main flow**
![Main Flow](/design/assets/UC-002.png)

Normal operations:
│   ├── UC-001: Select and open a PDF
│   ├── UC-002: Read and navigate a PDF
│   ├── UC-003: Power on the device
│   └── UC-004: Power off the device from menu
│   └── UC-004: Power off the device from book
│
├── 5. Exceptional Operations
│   ├── UC-005: Respond to low battery
│   └── UC-006: Handle insufficient battery power
│
├── 6. Maintenance Operations
│   └── UC-007: Diagnose a device failure automatically
│
├── 7. Integration and Testing
│   └── Initial assembly and acceptance testing
│
├── 8. Commercialization Considerations
│   ├── Manufacturing cost versus production volume
│   ├── Gross margin at different selling prices
│   └── Cost assumptions and uncertainties



Use cases we want to cover:
- OBJ-User-01: Selecting a PDF, reading a PDF, changing page in PDF.
- OBJ-User-02: Powering on ebook reader from battery, warning that battery is low, not enough battery to operate mode, poweroff and power on.
- OBJ-DeveloperMantainer-04: Diagnose a device failure without external hardware.
- OBJ-ProjectOwner-06: Graph showing on x axis manufacturing cost and on Y axis number of manufactured devices.
- OBJ-ProjectOwner-08: Graph shjowing on x  manufacturing cost and on Y axis gross margin, make few version each for one selling price.
- OBJ-ProjectOwner-12: Create a list of assumptions made in calculating costs.

