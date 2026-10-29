Stakeholders
=================

This document identifies the stakeholders of the **Open Source E-book Reader** project, their expectations, and the needs, goals, and objectives that guide its development.

The approach is inspired by the *NASA Systems Engineering Handbook*.

The project is developed by [Jakub Buczyński](https://github.com/KubaTaba1uga), referred to below as the `@KubaTaba1uga`.


## 1. Purpose

Before defining technical requirements, we need to understand **who the system is intended to serve, what they expect from it, and why those expectations matter**.

Stakeholder analysis helps us:

- Identify the people and groups affected by the project.
- Understand their expectations and priorities.
- Establish the project's needs, goals, and objectives.
- Identify conflicting interests and engineering trade-offs.
- Provide a basis for developing operational scenarios and technical requirements.

Stakeholder analysis is an iterative process. Stakeholders and their expectations and objectives may change as the project evolves.


## 2. Stakeholder identification

A **stakeholder** is an individual or group that has an interest in the project.

The following fours stakeholder roles have been identified:
- [User](#STAKE-User)
- [Potential customer](#STAKE-PotentialCustomer)
- [Developer/Mantainer](#STAKE-DeveloperMantainer)
- [Project Onwer](#STAKE-ProjectOwner)

The project author currently occupies several stakeholder roles. Consequently, the project serves multiple purposes: developing a usable e-book reader, improving engineering skills, demonstrating engineering competence, and evaluating commercial potential of hackable ebook device.

These interests are not always aligned and must be considered when making engineering decisions.


## 3. Stakeholder analysis

Each stakeholder has assigned excpectations, each excpectation has one of four priorities:
- Must
- Should
- Could
- Won't

The levels can be compared with each other according to follwoing hirearchy: 
```
Must > Should > Could > Won't
```

Each expectations conatining `Must` has to be implemented to call project succesfull.


### User

**ID**: <a id="STAKE-User"></a>STAKE-User

Technically inclined individual who want to read books on a dedicated, customizable device.

| ID          | Expectation                                                                           | Priority |
| ----------- | ------------------------------------------------------------------------------------- | -------- |
| EXP-User-01 | Read ordinary text in PDF documents reliably.                                         | Must     |
| EXP-User-02 | Use the device without an external power cord.                                        | Must     |
| EXP-User-03 | Display and change PDF pages responsively.                                            | Must     |
| EXP-User-04 | Display PDF tables, diagrams, and images correctly without losing important content.  | Should   |
| EXP-User-05 | Carry and use the device comfortably while traveling.                                 | Could    |
| EXP-User-06 | Read ordinary text in EPUB format.                                                    | Could    |
| EXP-User-07 | Display EPUB tables, diagrams, and images correctly without losing important content. | Could    |
| EXP-User-08 | Support reading in the dark.                                                          | Could    |


### Potential Customer

**ID**: <a id="STAKE-PotentialCustomer"></a>STAKE-PotentialCustomer

Someone evaluating whether they would purchase a future consumer-grade version of the device.

At the current stage, the project is only a prototype. A decision on whether to pursue a consumer-grade version has not yet been made.

| ID                       | Expectation                                                       | Priority |
|--------------------------|-------------------------------------------------------------------|----------|
| EXP-PotentialCustomer-01 | Understand the device's limitations before purchasing it.         | Must     |
| EXP-PotentialCustomer-02 | Have access to the reading capabilities that are important to me. | Should   |
| EXP-PotentialCustomer-03 | Obtain good value compared with alternative e-readers.            | Could    |
| EXP-PotentialCustomer-04 | Compare the device with alternatives such as Kindle and Kobo.     | Could    |


### Developer / Maintainer

**ID**: <a id="STAKE-DeveloperMaintainer"></a>STAKE-DeveloperMaintainer

**Role holder**: `@KubaTaba1uga`

A person responsible for implementing, integrating, debugging, modifying, and maintaining the device's hardware and software.

| ID                         | Expectation                                                                               | Priority |
|----------------------------|-------------------------------------------------------------------------------------------|----------|
| EXP-DeveloperMaintainer-01 | Extend the software easily in the future.                                                 | Must     |
| EXP-DeveloperMaintainer-02 | Isolate, diagnose, and repair hardware and software failures without excessive effort.    | Should   |


### Project Owner

**ID**: <a id="STAKE-ProjectOwner"></a>STAKE-ProjectOwner

**Role holder**: `@KubaTaba1uga`

A person responsible for defining project objectives, managing scope, allocating resources, accepting risks, and determining whether the project has succeeded.

| ID                  | Expectation                                                                             | Priority |
|---------------------|-----------------------------------------------------------------------------------------|----------|
| EXP-ProjectOwner-01 | Estimate the manufacturing cost and potential selling price of a consumer-grade device. | Must     |
| EXP-ProjectOwner-02 | Determine whether producing and selling the device could be commercially viable.        | Must     |
| EXP-ProjectOwner-03 | Ensure the device design supports practical assembly, testing, and maintenance.         | Could    |


## 4. Needs, goals, objectives and requirements

We specify needs, goals and objectives based on the stakeholder excpectation.

Especially goals and objectives will be usefull while creating use cases and requirements.

You can imagine objectives as the direction we want to pursue. Use cases as descriptions how we achieve the direction,
and requirements as a measure describing when the end destination is met. 

Let's examine few examples: Objective is "Travel from poland to italy", Use case is we travel by car and travel frew Czechia and Austria, and requirement may be reaching town in Italy called Rome. Maybe more technical one  objective `Read ordinary PDF text reliably`, which will lead us to the use case of battery powered device and how one can read a PDF page by page and later will lead us to requirements like `Change page in X seconds`.


### Independent PDF reading
<a id="NEED-User-01"></a>

User need to read PDF documents without relying on a computer.

<a id="GOAL-User-01"></a>
**Goal**

Provide a self-contained device suitable for everyday PDF reading.

**Objectives**

- <a id="OBJ-User-01"></a> `OBJ-User-01`: Read ordinary PDF text reliably.
- <a id="OBJ-User-02"></a> `OBJ-User-02`: Operate without external power.
- <a id="OBJ-User-03"></a> `OBJ-User-03`: Provide a responsive GUI.
- <a id="OBJ-User-04"></a> `OBJ-User-04`: Read tables, diagrams, and images.
- <a id="OBJ-User-05"></a> `OBJ-User-05`: Minimize device size.

| Objective ID | Expectation ID |
| ------------ | -------------- |
| OBJ-User-01  | EXP-User-01    |
| OBJ-User-02  | EXP-User-02    |
| OBJ-User-03  | EXP-User-03    |
| OBJ-User-04  | EXP-User-04    |
| OBJ-User-05  | EXP-User-05    |


### Acknowledge device limitations
<a id="NEED-PotentialCustomer-01"></a>

Potential customer need to understand limitations of the e-book reader to perform meaningfull comparision with alternative solutions.

<a id="GOAL-PotentialCustomer-01"></a>
**Goal**

- <a id="OBJ-PotentialCustomer-01"></a> `OBJ-PotentialCustomer-01` Identify capabilities the device does not support.
- <a id="OBJ-PotentialCustomer-02"></a> `OBJ-PotentialCustomer-02` Document known technical limitations.
- <a id="OBJ-PotentialCustomer-03"></a> `OBJ-PotentialCustomer-03` Communicate these limitations clearly to users and potential customers.

| Objective ID              | Expectation ID           |
| ------------------------- | ------------------------ |
| OBJ-PotentialCustomer-01  | EXP-PotentialCustomer-01 |
| OBJ-PotentialCustomer-02  | EXP-PotentialCustomer-01 |
| OBJ-PotentialCustomer-03  | EXP-PotentialCustomer-01 |


### Killer feature
<a id="NEED-PotentialCustomer-02"></a>

Potential customer need the e-book reader to deliver exceptional value in a narrowly defined use case rather than provide many somehow implemented features.

<a id="GOAL-PotentialCustomer-02"></a>
**Goal**

Identify the primary use case for the device keeping in mind cost and time restrictions.

**Objectives**

- <a id="OBJ-PotentialCustomer-04"></a> `OBJ-PotentialCustomer-04` Determine how the intended users want to use the e-book reader.
- <a id="OBJ-PotentialCustomer-05"></a> `OBJ-PotentialCustomer-05` Select one primary capability to receive development priority.
- <a id="OBJ-PotentialCustomer-06"></a> `OBJ-PotentialCustomer-06` Define what successful operation of the selected capability means.
- <a id="OBJ-PotentialCustomer-07"></a> `OBJ-PotentialCustomer-07` Demonstrate the capability through representative user scenarios.
- <a id="OBJ-PotentialCustomer-08"></a> `OBJ-PotentialCustomer-04` Evaluate feedback against the project's primary use case.

| Objective ID             | Expectation ID           |
| ------------------------ | ------------------------ |
| OBJ-PotentialCustomer-04 | EXP-PotentialCustomer-02 |
| OBJ-PotentialCustomer-05 | EXP-PotentialCustomer-02 |
| OBJ-PotentialCustomer-06 | EXP-PotentialCustomer-02 |
| OBJ-PotentialCustomer-07 | EXP-PotentialCustomer-02 |
| OBJ-PotentialCustomer-08 | EXP-PotentialCustomer-02 |


### Extendability and mantainability
<a id="NEED-DeveloperMantainer-01"></a>

Developer/Mantainer need to extend and mantain the device easily.

<a id="GOAL-DeveloperMantainer-01"></a>
**Goal**

Provide a maintainable and extensible hardware/software system that supports efficient development, troubleshooting, and modification.

**Objectives**

- <a id="OBJ-DeveloperMantainer-01"></a> `OBJ-DeveloperMantainer-01`: Enable software functionality to be modified or extended with limited impact on unrelated functionality.
- <a id="OBJ-DeveloperMantainer-02"></a> `OBJ-DeveloperMantainer-02`: Ensure subsystem interfaces and dependencies are sufficiently documented to support integration and maintenance.
- <a id="OBJ-DeveloperMantainer-03"></a> `OBJ-DeveloperMantainer-03`: Enable hardware and software subsystems to be tested independently where practical.
- <a id="OBJ-DeveloperMantainer-04"></a> `OBJ-DeveloperMantainer-04`: Enable faults to be isolated to a specific subsystem or interface.
- <a id="OBJ-DeveloperMantainer-05"></a> `OBJ-DeveloperMantainer-05`: Enable individual hardware components to be replaced without redesigning unrelated subsystems.

| Objective ID               | Expectation ID            |
| -------------------------- | ------------------------- |
| OBJ-DeveloperMantainer-01  | EXP-DeveloperMantainer-01 |
| OBJ-DeveloperMantainer-02  | EXP-DeveloperMantainer-01 |
| OBJ-DeveloperMantainer-03  | EXP-DeveloperMantainer-02 |
| OBJ-DeveloperMantainer-04  | EXP-DeveloperMantainer-02 |
| OBJ-DeveloperMantainer-05  | EXP-DeveloperMantainer-02 |


### Commercialization
<a id="NEED-ProjectOwner-01"></a>

Project Onwer need to determine whether developing a consumer-grade version of the e-book reader is a commercially worthwhile investment.

<a id="GOAL-ProjectOwner-01"></a>
**Goal 1**

Evaluate potential customer demand.

**Objectives 1**

- <a id="OBJ-ProjectOwner-01"></a> `OBJ-ProjectOwner-01`: Present the proposed product's capabilities, limitations, and intended benefits to potential customers.
- <a id="OBJ-ProjectOwner-02"></a> `OBJ-ProjectOwner-02`: Collect indications of purchasing interest from prospective customers.
- <a id="OBJ-ProjectOwner-03"></a> `OBJ-ProjectOwner-03`: Estimate how interest varies with proposed selling price.
- <a id="OBJ-ProjectOwner-04"></a> `OBJ-ProjectOwner-04`: Measure the number of interested customers over a defined evaluation period.
- <a id="OBJ-ProjectOwner-05"></a> `OBJ-ProjectOwner-05`: Identify limitations and potential biases in the collected evidence.

<a id="GOAL-ProjectOwner-02"></a>
**Goal 2**

Evaluate financial feasibility to support a decision on whether to pursue commercialization

**Objectives 2**

- <a id="OBJ-ProjectOwner-06"></a> `OBJ-ProjectOwner-06`: Estimate manufacturing cost per device at different production volumes.
- <a id="OBJ-ProjectOwner-07"></a> `OBJ-ProjectOwner-07`: Estimate non-manufacturing costs, including development, distribution, support, and other relevant expenses.
- <a id="OBJ-ProjectOwner-08"></a> `OBJ-ProjectOwner-08`: Estimate potential selling prices, gross margins, and break-even volumes.
- <a id="OBJ-ProjectOwner-09"></a> `OBJ-ProjectOwner-09`: Evaluate the sensitivity of profitability to production volume, component prices, and selling price.
- <a id="OBJ-ProjectOwner-10"></a> `OBJ-ProjectOwner-10`: Compare estimated customer willingness to pay with projected costs and selling prices.
- <a id="OBJ-ProjectOwner-11"></a> `OBJ-ProjectOwner-12`: Identify major commercial risks and uncertainties.
- <a id="OBJ-ProjectOwner-12"></a> `OBJ-ProjectOwner-12`: Produce an evidence-based recommendation on whether to proceed, revise the concept, or stop commercialization efforts.

| Objective ID         | Expectation ID      |
| -------------------- | ------------------- |
| OBJ-ProjectOwner-01  | EXP-ProjectOwner-02 |
| OBJ-ProjectOwner-02  | EXP-ProjectOwner-02 |
| OBJ-ProjectOwner-03  | EXP-ProjectOwner-02 |
| OBJ-ProjectOwner-04  | EXP-ProjectOwner-02 |
| OBJ-ProjectOwner-05  | EXP-ProjectOwner-02 |
| OBJ-ProjectOwner-06  | EXP-ProjectOwner-02 |
| OBJ-ProjectOwner-07  | EXP-ProjectOwner-01 |
| OBJ-ProjectOwner-08  | EXP-ProjectOwner-01 |
| OBJ-ProjectOwner-09  | EXP-ProjectOwner-01 |
| OBJ-ProjectOwner-10  | EXP-ProjectOwner-01 |
| OBJ-ProjectOwner-11  | EXP-ProjectOwner-01 |
| OBJ-ProjectOwner-12  | EXP-ProjectOwner-01 |
| OBJ-ProjectOwner-13  | EXP-ProjectOwner-01 |

