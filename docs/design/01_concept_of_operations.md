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

**Related objectives**: [OBJ-User-02](/design/00_stakeholders.md#OBJ-User-02)

**Preconditions**: Device is powered off.

**Postconditions**: The device either start ebook reader application or reports a startup failure and powers off.

**Trigger**: User presses the power button.

**Main flow**
![Main Flow](/design/assets/UC-001.png)

We need to indicate to user that we received his button press, otherwise he may think that nothing is hapenning and that device is not working.

We need checking operation conditions because if device is not safe to operate we do not want to start ebook reader application which may write to 
disk and make some important files unusable in case of failure. For example battery level, in case of battery level we want to show sth like
battery too low and power off. Beside battery, we may want to try reach the screen and check whether it is responding.

In battery ctx, displaying something on epaper screen draws a lot of current so it may be wise to first check battery and then decide whether 
proceed with the startup process.

### Select and open a book

**ID**: <a id="UC-002"></a>UC-002

**Related objectives**: [OBJ-User-01](/design/00_stakeholders.md#OBJ-User-01), [OBJ-User-03](/design/00_stakeholders.md#OBJ-User-03)

**Preconditions**: All hardware components are intialized correctly and are working. Ebook reader application is started.

**Postconditions**: Book's page is displayed on the screen or app showed error and handled it.

**Trigger**: Application is started.

**Main flow**
![Main Flow](/design/assets/UC-002.png)

You can open an book from multiple places, menu, library or in initialization. Same in error state, basically every statre can go to error.
Which means that all recorvable errors should be handled internally by particullar states. It is important because errors considering books 
(ex. not supported format) need to be considered in design of each state of the app.


### Read and navigate a PDF



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

