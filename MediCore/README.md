# MediCore - Hospital Management System

## Overview
A Hospital Management System built in C++17 with SFML 3.0.2 GUI.
Supports Patient, Doctor, and Admin roles with persistent text-file storage.

## Prerequisites

### macOS - Apple Silicon
```bash
xcode-select --install
brew install cmake sfml
```


## Build and Run on macOS
```bash
git clone https://github.com/Abdul-Haseeb070/MediCore.git
cd MediCore
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.logicalcpu)
./MediCore
```
```

## Default Credentials
| Role    | ID | Password |
|---------|----|----------|
| Patient | 1  | pass123  |
| Patient | 2  | sara456  |
| Doctor  | 1  | doc123   |
| Doctor  | 2  | doc456   |
| Admin   | 1  | admin123 |

## Class Relationships (ASCII)
```
Person (abstract)
|- Patient
|- Doctor
`- Admin

Appointment
Bill
Prescription

Storage<T>
FileHandler
Validator

HospitalException
|- FileNotFoundException
|- InsufficientFundsException
|- InvalidInputException
`- SlotUnavailableException
```
