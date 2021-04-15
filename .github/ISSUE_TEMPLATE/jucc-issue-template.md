---
name: JuCC Issue Template
about: Please follow this template when creating an issue for JuCC!
title: ''
labels: ''
assignees: ''

---

Welcome to the issue tracker for **JuCC**! We're excited that you're interested in improving our system. Below, please **choose either a Feature Request or Bug Report** and replace the sample text below to describe the issue! Additionally, please choose the appropriate labels on the Github panel. If you wish to and are able to solve the issue, feel free to assign yourself; otherwise we will manage this later!

# Feature Request
## Summary
Please provide a short summary of the feature you would like implemented.

## Solution
If possible, include a description of the desired solution you have in mind. Ideally, a series of steps outlining what is required and a plan to implement would be the most helpful for our developers!

Feel free to suggest more or raise concerns with the existing items. When we're convinced of the list, we should make an overall project with issues for each item in the list to spread out the tasks and track progress.

### Alternatives
If you are aware of any alternatives to the solution you presented, please describe them here!

___

# Bug Report
**Note**: Before filing a bug report, please make sure to check whether the bug has already been filed. If it has, please do not re-file the report, our developers are already hard at work fixing it!

## Summary
Please provide a short summary of the bug observed.

## Environment
To address the bug, especially if it environment specific, we need to know what kind of configuration you are running on. Please include the following:

**OS**: Ubuntu (LTS) 20.04 or macOS 10.14+ (please specify version).

**Compiler**: GCC 7.0+ or Clang 8.0+. 

**CMake Profile**: `Debug`, `Release`, `FastDebug`, or `Relwithdebinfo`. If exists across all cmake profiles on a platform-compiler combo, then please say so.

**CI**: If the bug has been observed in CI, please link to the CI build so that the bug can be referenced. Please make sure that if the issue has appeared on a PR branch that the PR itself is not causing the issue.

## Steps to Reproduce
Whenever possible, retrace how you came about the problem and describe so here.

If you have an understanding of why the bug occurred, that would be awesome to include as well! In this case, be as descriptive as you can and provide references to lines of code in the code whenever possible!

### Expected Behavior
Fill in the behavior expected from our system, as well as the reference you used.

### Actual Behavior
Fill in the behavior you actually observed for our system.
