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
| Postconditions     | What should be true when the scenario finishes        |
| Trigger            | What starts the scenario                              |
| Main flow          | Normal sequence of actor actions and system responses |


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

The device may be powered off without a control, and we plan to ensure best effort recorvability from that, we may need to add to verification step 
these recovery methods. 

### Select and open a book

**ID**: <a id="UC-002"></a>UC-002

**Related objectives**: [OBJ-User-01](/design/00_stakeholders.md#OBJ-User-01), [OBJ-User-03](/design/00_stakeholders.md#OBJ-User-03)

**Preconditions**: All hardware components are intialized correctly and are working. Ebook reader application is started.

**Postconditions**: Book's page is displayed on the screen or app showed error and handled it.

**Trigger**: Application is started.

**Main flow**
![Main Flow](/design/assets/UC-002.png)

You can open an book from multiple places, menu, library or in initialization. 

Same in error state, basically every statre can go to fatal error procedure.

All recorvable errors should be handled internally by particullar states. 
It is important because errors considering books (ex. not supported format) need to be considered in design of each state of the app.


### Read and navigate a PDF

**ID**: <a id="UC-003"></a>UC-003

**Related objectives**: [OBJ-User-01](/design/00_stakeholders.md#OBJ-User-01), [OBJ-User-04](/design/00_stakeholders.md#OBJ-User-04), [OBJ-User-03](/design/00_stakeholders.md#OBJ-User-03)

**Preconditions**: Book's page is displayed on the screen.

**Postconditions**: Book's page or menu is displayed on the screen.

**Trigger**: Book is opened.

**Main flow**:
![Main Flow](/design/assets/UC-003.png)

This is one of the most important flows, rendering the page is propably the most CPU consuming and the most frequent operation, so we need to design it carefully.
When user request next or previous page the change should be well benchmarked and measured, this is where we should focus our optimization on.

We did some research regarding only PDF, we will display PDF as photos, so we should allow setting zoom and camera position.
Different formats can require however different settings so we should to remember that zoom and camera position are not general per book but rather per format.

We need to be carefull during optimization of render, cause when user change settings of the book it influence how the book should look like on the screen.
We should minimize influence of zoom camera to render's time. 

What are alternatives to displaying PDF as photo per page?

What PDFs should we benchmark on? Does it metter what is the content on the pdf (propably yes)?

Is there better format to store pictures than PNG? What are alternatives? Does format affect performance?

What happens when the user requests the next page while already on the last page?
What happen when user requests the previous page while viewing the first page?
What happen when user requests the invalid zoom or camera position?
What happen when the PDF contains a very large image or complicated vector graphics?
What happen when rendering fails or takes an unexpectedly long time?
What happen when the device runs out of available memory while processing a PDF?


### Power Off the device

**ID**: <a id="UC-004"></a>UC-004

**Related objectives**: [OBJ-User-02](/design/00_stakeholders.md#OBJ-User-02)

**Preconditions**: Device is powered on. Application is opened.

**Postconditions**: Device is powered off.

**Trigger**: Multiple.

**Main flow**
![Main Flow](/design/assets/UC-004.png)

The power off can be initiated for multiple reasons:
 - User can poweroff the device using poweroff button.
 - Application can record low battery and schedule poweroff.
 - The device can be powered off due to failure in detecting low battery, in which case we should try not to invalidate the data.
 
Additionally user can be in one of many places while poweroff is being scheduled:
 - In menu
 - In reader
 - In reader in book settings
 - In library
We should have some tests to ensure that poweroff is handled everywhere correctly.
 
It may be usefull to consider our storage (SD card), our DB (SQLite), and our current filesystem format (EXT2) 
to determine whether we can do sth better to not loose data integrity in case of sudden poweorff. 


### Select one primary capability to receive development priority

**ID**: <a id="UC-005"></a>UC-005

**Related objectives**: [OBJ-PotentialCustomer-04](/design/00_stakeholders.md#OBJ-PotentialCustomer-04) [OBJ-PotentialCustomer-05](/design/00_stakeholders.md#OBJ-PotentialCustomer-05) [OBJ-PotentialCustomer-06](/design/00_stakeholders.md#OBJ-PotentialCustomer-06) [OBJ-PotentialCustomer-07](/design/00_stakeholders.md#OBJ-PotentialCustomer-07)
						
**Preconditions**: None.

**Postconditions**: Primary capabality selected and included in ConOps.

**Trigger**: None.

**Main flow**:
1. List capabilities that we can deliver in aproximatly a month or two.
2. Evaluate capabilities against user value, implementation effort, technical risk, and evidence of customer interest.
3. Select one capability.
4. Define what successful operation of the selected capability means.
5. Include capability in ConOps.

Capabilities list:
- Multiple formats support (EPUB, MOBI)
- PDF Words Translation
- Performance
- Long battery life
- Good responsivness


### Create project site where we gather customer interest

**ID**: <a id="UC-006"></a>UC-006

**Related objectives**: [OBJ-ProjectOwner-02](/design/00_stakeholders.md#OBJ-ProjectOwner-02) [OBJ-ProjectOwner-03](/design/00_stakeholders.md#OBJ-ProjectOwner-03) [OBJ-ProjectOwner-04](/design/00_stakeholders.md#OBJ-ProjectOwner-04)
						
**Preconditions**: None.

**Postconditions**: Exsist a website which is a go to place regareding all stuff regarding the project and form with user questions.

**Trigger**: Defined project is finished.

**Main flow**:
1. We create a website which shows pictures and videos of the ebook reader.
2. We list all most important info on website like link to docs, link to repo, how aquire device, plans for future of the project, why we develop it etc.
3. We list current project limitations.
4. We provide Form of Interest where user can express interest in buying the device, how much he/she would pay (100, 500, 1000 more) and what features are important (long battery live, touch screen, front ligth, big screen, wifi, hackability, customizibility, good responsivness, built in translation of books). We need to also ask about the measure of interest wont but, maybe will buy, will buy for sure etc. Maybe we cand o some concept graphic how it could look like?
5. We write articles about the ebook reader and publicize to gather some users data.
6. We gather user data.
7. We analyze anwsers to find how many people each day expressed interest and in what wariant. We need to store all anwserts it's really important!


The idea is to create one place where we place links to repos, photos videos, links to articles about the ebook reader.
I want people to understand what the idea is about, open source ebook reader. You can build your sdcard and do any customization you want,
from changing stuff in 3d printed enclosure to changing stuff on pcb board or in the linux application.

I want people to see currtent state of development and what we can squueze out from current hardware in POC.

I want people to understand that this si not yet finished device and that whether i pursue real device depends a lot on the data in the form on my website.

I need some way to track how much people go on the main page every week. Maybe some website with graph where i can go every day, and which has marked publications dates.

At this point we have only protype and contemplating whether pursu building customer grade device. Now we have POC, we do not have a product.


<!-- UC-007 — Import a PDF document	How do books reach the device? -->
<!-- UC-008 — Diagnose a device failure	Supports your maintainer objective -->
<!-- UC-009 — Initial device setup	Useful for integration and acceptance testing -->

<!-- Normal operations: -->
<!-- │   ├── UC-001: Select and open a PDF -->
<!-- │   ├── UC-002: Read and navigate a PDF -->
<!-- │   ├── UC-003: Power on the device -->
<!-- │   └── UC-004: Power off the device from menu -->
<!-- │   └── UC-004: Power off the device from book -->
<!-- │ -->
<!-- ├── 5. Exceptional Operations -->
<!-- │   ├── UC-005: Respond to low battery -->
<!-- │   └── UC-006: Handle insufficient battery power -->
<!-- │ -->
<!-- ├── 6. Maintenance Operations -->
<!-- │   └── UC-007: Diagnose a device failure automatically -->
<!-- |-------UC-008: Develop ebook reader app on local machine -->
<!-- |-------UC-008: Diagnose regression in app automatically -->
<!-- | -->
<!-- ├── 7. Integration and Testing -->
<!-- │   └── Initial assembly and acceptance testing -->
<!-- │ -->
<!-- ├── 8. Commercialization Considerations -->
<!-- │   ├── Manufacturing cost versus production volume -->
<!-- │   ├── Gross margin at different selling prices -->
<!-- │   └── Cost assumptions and uncertainties -->



<!-- Use cases we want to cover: -->
<!-- - OBJ-User-01: Selecting a PDF, reading a PDF, changing page in PDF. -->
<!-- - OBJ-User-02: Powering on ebook reader from battery, warning that battery is low, not enough battery to operate mode, poweroff and power on. -->
<!-- - OBJ-DeveloperMantainer-04: Diagnose a device failure without external hardware. -->
<!-- - OBJ-ProjectOwner-06: Graph showing on x axis manufacturing cost and on Y axis number of manufactured devices. -->
<!-- - OBJ-ProjectOwner-08: Graph shjowing on x  manufacturing cost and on Y axis gross margin, make few version each for one selling price. -->
<!-- - OBJ-ProjectOwner-12: Create a list of assumptions made in calculating costs. -->
<!-- - OBJ-ProjectOwner-12: Show somehow that we need to optimize for getting data from users whether they would purchase the device. -->

