# Torpedo-Vorhaltrechner-Project
This is a repo dedicated to building on the work done by Maciek Florek with his Torpedo Vorhaltrechner Project

# WW2 Torpedo Data Computer – Unreal Engine Simulation

A C++ / Unreal Engine reconstruction of a World War II-era torpedo data computer, built from historical research and reverse-engineered reference material, with simulation-focused behavior and real-time integration.

---

## Overview

This project is a reconstruction of a WW2 mechanical fire control system (torpedo data computer), implemented in C++ and integrated into Unreal Engine.

The goal was not just to reproduce the core calculations, but to build a system that behaves more like a real machine within a simulation environment — with defined inputs, derived outputs, and time-based response.

This project sits at the intersection of:
- historical engineering research  
- mechanical system interpretation  
- simulation design  
- real-time visualization in Unreal Engine  

---

## Background

This work originated from an interest in WW2-era naval engineering and archival ship blueprints from sources such as NARA and the Bundesarchiv.

Many of the systems developed during this period represent a high point of mechanical engineering — solving complex targeting and ballistic problems through gears, cams, discs, and linkages rather than digital computation.

This project focuses specifically on reconstructing aspects of the Torpedo Data Computer (TDC), based on reverse-engineered research and reference implementations.

---

## Features

The current implementation includes:

- Core targeting / triangle math
- Angle-on-the-bow (AOB) handling
- Gyro angle generation
- Torpedo spread calculation
- Parallax correction
- Time-based mechanical response simulation
- Defined input / output structure
- Runtime behavior + editor-time integration in Unreal Engine

The system is structured to be modular and inspectable, rather than a single monolithic calculation block.

---

## Implementation Details

- Language: C++
- Engine: Unreal Engine
- Focus: real-time simulation and system behavior

The original reference implementation demonstrated the core calculations.  
This version expands that into a simulation-oriented system:

- Logic is separated into reusable components  
- Behavior is structured around inputs → processing → outputs  
- Designed to support interaction, visualization, and debugging  
- Extended beyond static math into time-dependent behavior  

The intent was to move from a **proof of concept** toward a more **simulation-grade system**.

---

## AI-Assisted Development

This project was developed with significant AI-assisted coding support using OpenAI Codex.

Codex was used to:
- help translate portions of the original logic into C++  
- assist with structuring and implementing components  

The following aspects were directed manually:
- system understanding and interpretation  
- architectural decisions  
- integration into Unreal Engine  
- debugging and validation  
- simulation behavior design  

AI was used as a tool to accelerate development, not as a substitute for system comprehension.

---

## Attribution

This project builds on reverse-engineering and research work by **Maciek Florek** related to the TV Re 3 / Torpedo Data Computer.

His published research and demonstration code were used as a foundational reference for this implementation.

Original reference:
http://www.tvre.org/pl/

If you are interested in the historical and technical background, his work is highly recommended.

---

## Purpose

The goal of this project is to explore how historically complex mechanical systems can be:

- understood through reconstruction  
- translated into modern software  
- made interactive in real-time environments  

It is also an experiment in bridging:
- historical research  
- mechanical engineering concepts  
- modern simulation tools  

---

## Status

This is an ongoing exploration project.

The system is functional and demonstrates key behaviors of a torpedo data computer, but is not intended to be a fully validated or historically complete reproduction.

---

## Media


---

## License

MIT License
