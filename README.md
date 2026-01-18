# IoT-DALI-Lighting-Control-System
IoT DALI Lighting Control System

This project is my Bachelor’s Thesis and focuses on the design and implementation of an IoT-based lighting control system using the DALI protocol.

The objective of the project is to build an end-to-end system that combines industrial lighting control, embedded hardware, energy monitoring, and cloud connectivity, providing remote supervision and control through a mobile application.

Project Overview

The system is based on a DALI lighting bus controlled by an embedded device connected to the cloud.
It allows monitoring electrical parameters, controlling lighting behavior, and visualizing data remotely.

The project covers the complete development cycle:

Hardware design

Embedded firmware

Cloud data storage

Mobile application

System-level integration

This repository contains the technical material developed during the project, including firmware, hardware design files, and system documentation.

System Architecture

At a high level, the system is structured as follows:

DALI Lighting Bus -> Embedded Controller (Wemos D1 mini)-> Energy Measurement Module -> Cloud Database (Firebase) -> Mobile Application

The embedded device communicates with the DALI bus, acquires energy consumption data, and sends relevant information to the cloud backend.
The mobile application retrieves this data and allows basic system control and visualization.

Main Components

Embedded System

Microcontroller: Wemos D1 mini (ESP8266)

DALI communication interface

Energy measurement using a dedicated sensor

Firmware developed using the Arduino framework

Hardware Design

Custom PCB integrating power regulation, communication, and measurement components

Schematics and PCB designed using EasyEDA

Backend

Cloud database based on Firebase

Storage of system states and energy data

Simple and lightweight data model focused on monitoring and visualization

Mobile Application

Developed using MIT App Inventor

Provides basic control functions and data visualization

Communicates with the cloud backend

Repository Structure

iot-dali-lighting-control-system/

firmware/ Embedded firmware

hardware/ Schematics and PCB design

backend/ Cloud backend structure

mobile-app/ Mobile application files

docs/ System documentation

Each folder contains a short description of its contents.

Design Approach

The project was developed with a system-oriented approach, focusing on:

Clear separation between hardware, firmware, and cloud components

Simple and robust communication flows

Practical implementation over theoretical complexity

The main objective was not to build a commercial product, but to understand and integrate all layers of a connected industrial system.

Current Status and Limitations

This project is a functional prototype developed in an academic context.
Aspects such as scalability, advanced security mechanisms, or industrial certification are outside the scope of the thesis, but could be addressed in future iterations.

Possible Improvements

Migration to a more scalable cloud architecture

Real-time dashboards

Alert and notification system

Improved mobile application user interface

Support for additional lighting control scenarios

Author

Juan Francisco Jiménez
Bachelor’s Degree in Electronic Engineering

This project was developed as part of my final degree work and reflects my interest in embedded systems, industrial automation, and connected systems.
