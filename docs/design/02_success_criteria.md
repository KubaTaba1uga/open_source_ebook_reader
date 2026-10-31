# Success Criteria

Measures of Effectiveness (MOEs) define the conditions under which stakeholders consider the project successful.

Each MOE is linked to a use case defined in the Concept of Operations (ConOps). Each use case is linked to one or more stakeholder objectives, preserving traceability between success criteria and stakeholder expectations.

Success criteria describe observable outcomes. Some criteria also serve as functional acceptance criteria or engineering performance measures.

## [Power On the Device](/design/01_concept_of_operations.md#UC-001)

- <a id="MOE-001"></a> `MOE-001`: The user receives visual confirmation that the device is starting within X seconds of pressing the power button.
- <a id="MOE-002"></a> `MOE-002`: The user can observe the current stage of the startup process.
- <a id="MOE-003"></a> `MOE-003`: The developer can measure the duration of each startup stage, including system verification.
- <a id="MOE-004"></a> `MOE-004`: If startup verification fails, the user receives an indication of the failure and its reason before the device powers off, where sufficient power and hardware functionality remain available.
- <a id="MOE-005"></a> `MOE-005`: The device prevents normal operation when the battery charge is below X.
- <a id="MOE-006"></a> `MOE-006`: The device starts the e-book reader application within X seconds of pressing the power button under normal operating conditions.

## [Select and Open a Book](/design/01_concept_of_operations.md#UC-002)

- <a id="MOE-007"></a> `MOE-007`: The user can successfully open and read at least 75% of representative PDF documents (minimum acceptable performance).
- <a id="MOE-008"></a> `MOE-008`: The user can successfully open and read at least 90% of representative PDF documents (target performance).
- <a id="MOE-009"></a> `MOE-009`: The user can open a selected PDF and see its first page within X seconds of selecting it.
- <a id="MOE-010"></a> `MOE-010`: The user can go from a powered-off device to reading a selected book within X seconds.
- <a id="MOE-011"></a> `MOE-011`: The user can read continuously for at least X hours using battery power without external power.
- <a id="MOE-012"></a> `MOE-012`: The user can access the five most recently opened books from the main menu, or all previously opened books if fewer than five exist.
- <a id="MOE-013"></a> `MOE-013`: The user can browse all books available in the device's library.
- <a id="MOE-014"></a> `MOE-014`: The user can import a representative book into the device's library within X seconds.

## [Read and Navigate a PDF](/design/01_concept_of_operations.md#UC-003)

- <a id="MOE-015"></a> `MOE-015`: The user can navigate to the next or previous PDF page within X seconds, measured from the button press until the requested page is displayed.
- <a id="MOE-016"></a> `MOE-016`: When PDF rendering fails or exceeds the permitted processing time, the device detects the failure and either recovers safely or initiates a controlled shutdown.
- <a id="MOE-017"></a> `MOE-017`: Before a controlled shutdown caused by a fatal rendering error, the user receives an indication of the failure and its reason, where technically possible.

## [Power Off the Device](/design/01_concept_of_operations.md#UC-004)

- <a id="MOE-018"></a> `MOE-018`: When battery charge falls below X, the user is informed of the low-battery condition and the device initiates a controlled shutdown.
- <a id="MOE-019"></a> `MOE-019`: The device completes or forces a controlled shutdown within X seconds of a shutdown request, regardless of the current application screen or state, provided the hardware remains operational.
- <a id="MOE-020"></a> `MOE-020`: After an unexpected power loss or interrupted shutdown, the device can restart and recover its stored data on a best-effort basis.
- <a id="MOE-021"></a> `MOE-021`: A normal controlled shutdown does not cause loss or corruption of previously saved user data.

## [Select One Primary Capability to Receive Development Priority](/design/01_concept_of_operations.md#UC-005)

- <a id="MOE-022"></a> `MOE-022`: The selected primary capability is documented in `00_stakeholders.md`, `01_concept_of_operations.md`, and `02_success_criteria.md`.
- <a id="MOE-023"></a> `MOE-023`: Feedback has been collected from at least X potential customers indicating whether the selected capability is important to them.

## [Create a Project Site to Gather Customer Interest](/design/01_concept_of_operations.md#UC-006)

- <a id="MOE-024"></a> `MOE-024`: Potential customers can access a public website containing information about the project, its current development status, and its intended capabilities.
- <a id="MOE-025"></a> `MOE-025`: The project owner has a documented plan for attracting potential customers to the project website.
- <a id="MOE-026"></a> `MOE-026`: Potential customers can express interest in purchasing the device through the project website.
- <a id="MOE-027"></a> `MOE-027`: For each published article or promotional post, the project owner can determine the number of website visitors and expressions of purchasing interest recorded during the following seven days.
- <a id="MOE-028"></a> `MOE-028`: The project website has received at least 10 expressions of purchasing interest.
- <a id="MOE-029"></a> `MOE-029`: At least four articles have been published and promoted on at least two social platforms, with at least one week between article publications.
- <a id="MOE-030"></a> `MOE-030`: Potential customer price expectations have been collected and are available for analysis.
- <a id="MOE-031"></a> `MOE-031`: Information about the capabilities and characteristics most important to potential customers has been collected and is available for analysis.
- <a id="MOE-032"></a> `MOE-032`: A publicly accessible list of known unsupported capabilities and technical limitations is available on the project website.

## [Design a Version of the Future Device](/design/01_concept_of_operations.md#UC-008)

- <a id="MOE-033"></a> `MOE-033`: Manufacturing cost per device has been estimated for at least three production volumes, including components, PCB manufacturing, assembly, and testing.
- <a id="MOE-034"></a> `MOE-034`: Development, distribution, customer support, and other relevant non-manufacturing costs have been estimated, with the assumptions documented.
- <a id="MOE-035"></a> `MOE-035`: Potential selling prices, gross margins, and break-even sales volumes have been calculated for at least three production scenarios.
- <a id="MOE-036"></a> `MOE-036`: Collected customer price expectations have been compared against estimated selling prices required to achieve profitability.
- <a id="MOE-037"></a> `MOE-037`: A written recommendation to proceed, revise the concept, or stop commercialization has been produced, supported by estimated costs, profitability, customer interest, and identified commercial risks.

