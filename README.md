# Cutting Edge — UE5 Combat System

![Combat Demo](README_Resources/demo_v3.gif)


Gameplay prototype demonstrating a modular melee combat system implemented in **Unreal Engine 5 using C++**.

The project focuses on implementing responsive combat mechanics, reusable gameplay systems, and scalable architecture using the **Unreal Gameplay Framework**.

---
# Gameplay Video

[Showcase](https://youtu.be/x7HhaHhOg9k)

The video demonstrates:

• Combo-based melee attacks
• Enemy combat interactions
• Dash ability
• Ultimate attack mechanic

---

# Project Overview

This prototype explores implementation of **action combat mechanics similar to modern third-person action games**.

The main focus of the project is not creating a full game, but building **reusable gameplay systems and combat architecture**.

Implemented systems include:

• Modular combat system
• Damage handling system
• Player abilities (dash / ultimate)
• Enemy combat interaction
• Gameplay architecture using Unreal Gameplay Framework

---

# Gameplay Systems

## Combat System

The combat system supports combo-based melee attacks and special abilities.

Features:

• Combo attack logic
• Damage calculation system
• Ability-based attacks (dash / ultimate)
• Interaction between player and enemy characters

---

## Ability System

The project implements a lightweight ability system inspired by **Gameplay Ability System concepts**.

Abilities include:

• Dash ability
• Ultimate attack ability

The system allows easy extension with additional abilities.

---

## Enemy Interaction

Enemy characters react to combat events triggered by the player.

Interactions include:

• Damage application
• Combat reaction logic
• Ability impact handling

---

# Technical Architecture

The project follows a modular architecture based on the **Unreal Gameplay Framework**.

Simplified structure:

```text
Character
 ├── CombatComponent
 ├── HealthComponent
 └── AbilitySystem
```

Core responsibilities:

Player Character
Handles player input and ability triggering.

Combat Component
Processes attacks, combos and damage logic.

Health Component
Tracks health and applies damage events.

Ability System
Handles player abilities such as dash and ultimate attacks.

---

# Technologies Used

Engine

• Unreal Engine 5

Systems

• Unreal Gameplay Framework
• Modular gameplay architecture
• Scalable damage and combat systems
• Doom-like attack token system
• Simple event system
• Attack wave system

Technologies

• Motion Warping
• Animation Notifies (and other Animation-related stuff such as Montages, AnimGraph, AnimBP, Blendspace, etc)
• AI Systems (BehaviorTree, AIController, EQS, AIDecorators, AITasks)
• Multicast Delegates
• Timelines
... and many more
---

# Project Goals

The main goals of this project were:

• Design modular gameplay combat architecture
• Implement responsive combat mechanics
• Explore scalable gameplay system design in Unreal Engine 5

---

# Repository Structure

```text
Source/
 └── CuttingEdge
     ├── Characters
     ├── Combat
     ├── Abilities
     └── Components
```

Gameplay logic is implemented primarily in the **Source** directory using C++.

---
