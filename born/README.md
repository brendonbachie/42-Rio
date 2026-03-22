*This project has been created as part of the 42 curriculum by <bgomes-b>.*

# Born2BeRoot

## Description

The **Born2BeRoot** project aims to introduce the fundamentals of **system administration** by setting up and configuring a secure virtual machine from scratch.  
The goal is to understand how an operating system works, how to manage users and permissions, apply security policies, and configure essential services.

In this project, a **Rocky Linux** virtual machine was installed and hardened following strict security and configuration rules defined by the subject.

## Project Description and Technical Choices

### Operating System Choice: Rocky Linux

Rocky Linux was chosen as the operating system for this project. It is a community-driven, enterprise-grade Linux distribution designed to be **100% bug-for-bug compatible with Red Hat Enterprise Linux (RHEL)**.

#### Pros:
- Enterprise-level stability
- Long-term support
- Strong focus on security
- Uses SELinux by default

#### Cons:
- Less beginner-friendly than Debian
- Smaller community compared to Debian/Ubuntu
- More rigid security policies

### Main Design Choices

- **Partitioning**
  - Separate partitions for `/`, `/home`, `/var`, `/tmp`, `/srv`, `/var/log`, `/boot`
  - Swap partition enabled
  - LVM used for flexibility and scalability

- **Security Policies**
  - SELinux enabled and set to *enforcing*
  - Strong password policies (expiration, complexity)
  - SSH configured to disable root login
  - Firewall enabled and restricted to required ports only

- **User Management**
  - Root user secured
  - Dedicated user created and added to `wheel` group
  - Sudo configured with strict rules and logging

- **Installed Services**
  - SSH
  - firewalld
  - sudo
  - chrony (time synchronization)
  - monitoring tools (as required by the subject)

## Comparisons

### Debian vs Rocky Linux

| Debian | Rocky Linux |
|------|------------|
| Community-driven | Enterprise-focused |
| AppArmor by default | SELinux by default |
| Easier for beginners | More secure by default |
| Faster setup | Stricter configuration |

### AppArmor vs SELinux

| AppArmor | SELinux |
|--------|---------|
| Path-based | Label-based |
| Easier to configure | More powerful and granular |
| Less strict | Enforcing by default |
| Used in Debian/Ubuntu | Used in RHEL/Rocky |

### UFW vs firewalld

| UFW | firewalld |
|----|-----------|
| Simple and user-friendly | Dynamic and flexible |
| Rule-based | Zone-based |
| Common in Debian | Default in Rocky |
| Less granular | Enterprise-grade |

### VirtualBox vs UTM

| VirtualBox | UTM |
|-----------|-----|
| Cross-platform | macOS focused |
| Widely used | Apple Silicon optimized |
| Better documentation | Simpler UI |
| Ideal for 42 projects | Limited features |

## Instructions

### Requirements
- VirtualBox
- Rocky Linux ISO

### Installation Steps
1. Create a new virtual machine in VirtualBox
2. Attach the Rocky Linux ISO
3. Configure CPU, RAM, and storage
4. Install Rocky Linux using minimal installation
5. Configure partitions using LVM
6. Set up users, sudo, and security rules
7. Enable and configure required services
8. Apply firewall and SELinux policies

### Execution
This project does not require execution.  
Evaluation is done by inspecting the system configuration directly on the virtual machine.

## Resources

### Documentation and References
- Rocky Linux Documentation
- Red Hat Enterprise Linux Documentation
- SELinux Project Documentation
- Linux man pages (`man sudo`, `man sshd`, `man firewalld`)
- 42 Born2BeRoot Subject PDF

### Use of AI
AI tools were used to:
- Clarify Linux administration concepts
- Compare operating systems and security tools
- Assist in understanding SELinux and firewall configurations
- Help structure and document the project (README)

No automated scripts or configurations were generated without understanding and manual validation.

---

## Final Notes

This project provided a strong foundation in Linux system administration, security best practices, and virtualization concepts, essential skills for any system engineer or developer.
