# 1. Script builder

- [1. Script builder](#1-script-builder)
- [2. Introduction](#2-introduction)
- [3. Installation](#3-installation)
- [3. Execution](#3-execution)
- [4. Test writing](#4-test-writing)
  - [4.1. Actions list section (#1)](#41-actions-list-section-1)
  - [4.2. Test section (#2)](#42-test-section-2)
  - [4.3. Parameters in action](#43-parameters-in-action)
  - [4.4. Parameters info](#44-parameters-info)
  - [4.5. Test info](#45-test-info)
  - [4.6. Tests list](#46-tests-list)
- [5. Authors](#5-authors)

# 2. Introduction

Scriptbuilder lets the user to write tests in a simple and safe way.
<br/><br/>

# 3. Installation

It is possible to select the Scriptbuilder compilation and install from cmake.
Take a look at https://github.com/robotology/blocktest/blob/master/README.md
for more details.

![alt text](../img/img003.png "Tarp and Scriptbuilder options.")

# 3. Execution

```bash
./Scriptbuilder
```

# 4. Test writing

In the following figure is possible to find the main UI section.

![alt text](img/img001.png "Scriptbuilder windows.")

## 4.1. Actions list section (#1)

Section **#1** is the action list. Here are available all the action
in the system divided by libraries.
It is possible to drag and drop the action to the test in section **2**.

## 4.2. Test section (#2)

Section **#2** contains the current test. It is possible to delete (del keyboard), copy and paste the action inside section or drag and drop action from section **#1**.

## 4.3. Parameters in action

Section **#3** shows all the parameters in the selected action in test
in selection **#2**.

## 4.4. Parameters info

Section **#4** shows all the parameter information in the selected parameter in selection **#3**.

## 4.5. Test info

Section **#5** shows all the selected test information.

## 4.6. Tests list

Section **#6** shows all available tests.

# 5. Authors
* Luca Tricerri ([*personal page*](http://www.iit.it/en/people/Luca-tricerri.html))


