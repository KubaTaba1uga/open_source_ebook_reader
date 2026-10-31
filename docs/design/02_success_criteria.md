Success criteria
======================

Measures of Effectiveness (MOEs) are the measures of success that are designed to correspond to 
accomplishment of the system objectives as defined by the stakeholder’s expectations. 
They are stated from the stakeholder’s point of view and represent criteria that are to be met
in order for the stakeholder to consider the project successful. 
As such, they can be synonymous with project success criteria.

We will try to link very MOE to particular use case. Because each use case is linked to
objective which is linked to  stakeholder expectation, the relation between MOE and
stakeholder is preserved.


### [Power On the device](/design/01_concept_of_operations.md#UC-001)

1. User can start the device and see application is starting in X seconds.
2. User see indicators of different startup process stages.
3. Developer can measure how many seconds took each step of boot, including verification.
4. User can see that verification failed and understand why.
User cannot use the device when battery is below X pct.
Time between user clicking power and device responsing in visual way take X seconds.

### [Select and open a book](/design/01_concept_of_operations.md#UC-002)

When we consider this feature implemented succesfully?

User can read at least 75% of representative PDFs.
User can read at least 90% of representative PDFs.
User can open a PDF page page in less than X seconds, from click to page displayed.
User can go from power off to open book in less than X seconds.
User can read continously for X hours on battery.
User can see last 5 books in menu.
User can browse all books in library.
User can import new book to the reader, and it take less than X seconds.


### [Read and navigate a PDF](/design/01_concept_of_operations.md#UC-003)

User can change a PDF page page in less than X seconds, from click to page displayed.
Device poweroff when rendering fails or takes an unexpectedly long time.
User can see reason of fatal error before poweroff take place.


### [Power off the device](/design/01_concept_of_operations.md#UC-004)

User is informed if battery fall below X procent and device poweroff.
User can poweroff the device in 100% scenarios. Application needs to detect stuff like it exit takes too long and exit anyway.
User do not loss data after power off which wasn't scheduled (Exit time out, Uncontrolled power loss). Device can detect that it happened and recover the data on best effort basis.


### [Select one primary capability to receive development priority](/design/01_concept_of_operations.md#UC-005)

Project owner can see new capability included in 00_stakeholders.md, 01_concept_of_operations.md, 02_success_criteria.md.
We have collected feedback from at least X potential customers indicating whether the selected feature is important to them.


### [Create project site where we gather customer interest](/design/01_concept_of_operations.md#UC-006)


Potential customer can go project website and read about the project. 
Project owner have a plan for attracting potential customers to the project website.
Potential customer can express interest in the project on project website. 
Project owner know that in period of week after posting article X , Y number of people went on the project website, J number of people expressed interest.
We have received at least 10 expressions of purchasing interest through the project website.
We have published at least four articles, promoted them on at least two social platforms, and maintained at least one week between publications.
We have collected potential customer price expectations.
We have collected info about what is important for potential customer.
There is publicly available limitations list of the project.



### [Design a version of future device](/design/01_concept_of_operations.md#UC-008)


We have estimated manufacturing cost per device for at least three production volumes, including component costs, PCB manufacturing, assembly, and testing.
We have estimated development, distribution, customer support, and other non-manufacturing costs, with assumptions documented.
We have calculated selling prices, gross margins, and break-even sales volumes for at least three production scenarios.
We have collected potential customer price expectations and compared them with estimated selling prices required to achieve profitability.
We have produced a written recommendation to proceed, revise the concept, or stop commercialization, supported by estimated costs, profitability, customer interest, and identified risks.

