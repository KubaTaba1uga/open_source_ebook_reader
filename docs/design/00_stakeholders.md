# Stakeholder Analysis

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

Stakeholder analysis is an iterative process. Stakeholders, expectations and objectives may change as the project evolves.


## 2. Stakeholder identification

A **stakeholder** is an individual or group that has an interest in the project.

The following six stakeholder roles have been identified:
- [User](#STAKE-User)
- [Potential customer](#STAKE-PotentialCustomer)
- [Developer/Mantainer](#STAKE-DeveloperMantainer)
- [Project Onwer](#STAKE-ProjectOwner)
- [My Portfolio Audience](#STAKE-MyAudience)
- [Engineering student](#STAKE-EngineeringStudent)

The project author currently occupies several stakeholder roles. Consequently, the project serves multiple purposes: developing a usable e-book reader, improving engineering skills, demonstrating engineering competence, and evaluating commercial potential of hackable ebook.

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

**ID**: <a id="STAKE-User">STAKE-User</a>

Technically inclined individual who want to read books on a dedicated, customizable device.

| ID          | Expectation                                                                          | Priority |
| ----------- | ------------------------------------------------------------------------------------ | -------- |
| EXP-User-01 | Read ordinary text in PDF documents reliably.                                        | Must     |
| EXP-User-02 | Use the device without an external power cord.                                       | Must     |
| EXP-User-03 | Display and change PDF pages responsively.                                           | Must     |
| EXP-User-04 | Display PDF tables, diagrams, and images correctly without losing important content. | Should   |
| EXP-User-05 | Carry and use the device comfortably while traveling.                                | Could    |


### Potential Customer

**ID**: <a id="STAKE-PotentialCustomer">STAKE-PotentialCustomer</a>

Someone evaluating whether they would purchase a future consumer-grade version of the device.

At the current stage, the project is only a prototype. A decision on whether to pursue a consumer-grade version has not yet been made.

| ID                       | Expectation                                                       | Priority |
|--------------------------|-------------------------------------------------------------------|----------|
| EXP-PotentialCustomer-01 | Obtain good value compared with alternative e-readers.            | Could    |
| EXP-PotentialCustomer-02 | Have access to the reading capabilities that are important to me. | Should   |
| EXP-PotentialCustomer-03 | Understand the device's limitations before purchasing it.         | Must     |
| EXP-PotentialCustomer-04 | Compare the device with alternatives such as Kindle and Kobo.     | Could    |


### Developer / Maintainer

**ID**: <a id="STAKE-DeveloperMaintainer">STAKE-DeveloperMaintainer</a>

**Role holder**: `@KubaTaba1uga`

A person responsible for implementing, integrating, debugging, modifying, and maintaining the device's hardware and software.

| ID                         | Expectation                                                                               | Priority |
|----------------------------|-------------------------------------------------------------------------------------------|----------|
| EXP-DeveloperMaintainer-01 | Isolate, diagnose, and repair hardware and software failures without excessive effort.    | Should   |
| EXP-DeveloperMaintainer-02 | Source replacement components throughout the intended maintenance lifetime of the device. | Should   |
| EXP-DeveloperMaintainer-03 | Extend the software easily in the future.                                                 | Must     |


### Project Owner

**ID**: <a id="STAKE-ProjectOwner">STAKE-ProjectOwner</a>

**Role holder**: `@KubaTaba1uga`

A person responsible for defining project objectives, managing scope, allocating resources, accepting risks, and determining whether the project has succeeded.

| ID                  | Expectation                                                                             | Priority |
|---------------------|-----------------------------------------------------------------------------------------|----------|
| EXP-ProjectOwner-01 | Estimate the manufacturing cost and potential selling price of a consumer-grade device. | Must     |
| EXP-ProjectOwner-02 | Determine whether producing and selling the device could be commercially viable.        | Must     |
| EXP-ProjectOwner-03 | Ensure the device design supports practical assembly, testing, and maintenance.         | Could    |


### My Audience

**ID**: <a id="STAKE-MyAudience">STAKE-MyAudience</a>

The audience interested in evaluating, understanding, or learning from the author's engineering work.

This includes:
- Potential employers or engineering consultancy customers.
- Technical readers interested in [my website](https://funwithcomputers.fyi).
- Community members who may follow my work or participate in discussions.

| ID                | Expectation                                                                                                              | Priority |
|-------------------|--------------------------------------------------------------------------------------------------------------------------|----------|
| EXP-MyAudience-01 | See objective evidence that engineering decisions are based on analysis, experimentation, and verification.              | Must     |
| EXP-MyAudience-02 | Understand the project's objectives, architecture, and major design decisions.                                           | Must     |
| EXP-MyAudience-03 | Learn about significant engineering problems encountered during development and how they were investigated and resolved. | Should   |


### Engineering Student

**ID**: <a id="STAKE-EngineeringStudent">STAKE-EngineeringStudent</a>

**Role holder**: `@KubaTaba1uga`

A person using the project to develop practical skills in embedded systems, electronics, systems engineering, and technical problem-solving.

| ID                        | Expectation                                                                                         | Priority |
|---------------------------|-----------------------------------------------------------------------------------------------------|----------|
| EXP-EngineeringStudent-01 | Understand the reasons behind successful and unsuccessful engineering decisions.                    | Must     |
| EXP-EngineeringStudent-02 | Gain practical experience designing and integrating embedded Linux systems.                         | Should   |
| EXP-EngineeringStudent-03 | Improve knowledge of electronic circuit design, power distribution, and hardware debugging.         | Should   |
| EXP-EngineeringStudent-04 | Practice systematic requirements development, architecture design, trade studies, and verification. | Should   |


## 4. Needs, goals, objectives and requirements

We specify needs, goals and objectives based on the stakeholder excpectation.

Especially goals and objectives will be usefull while creating use cases and while creating requirements.

You can imagine objectives as the direction we want to pursue. Use cases as descriptions how we achieve the direction,
and requirements as a measure describing when the end destination is met.

For example we specify objective `Read ordinary PDF text reliably`, which will lead us to the use case of reading a PDF and later will lead us to requirements like `Change page in X seconds`.

### Independent PDF reading
<a id="NEED-User-01"></a>

Read PDF documents without relying on a computer.

<a id="GOAL-User-01"></a>
**Goal**

Provide a self-contained device suitable for everyday PDF reading.

**Objectives**

- <a id="OBJ-User-01"></a> `OBJ-User-01`: Read ordinary PDF text reliably.
- <a id="OBJ-User-02"></a> `OBJ-User-02`: Operate without external power.
- <a id="OBJ-User-03"></a> `OBJ-User-03`: Provide a responsive GUI.
- <a id="OBJ-User-04"></a> `OBJ-User-04`: Read tables, diagrams, and images.

| Objective ID | Expectation ID |
| ------------ | -------------- |
| OBJ-User-01  | EXP-User-01    |
| OBJ-User-02  | EXP-User-04    |
| OBJ-User-01  | EXP-User-03    |
| OBJ-User-01  | EXP-User-04    |


