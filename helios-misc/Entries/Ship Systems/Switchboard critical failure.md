---
status : 2
securityClass: 1
Name: Switchboard critical failure
Keywords:  Ship systems, warfare, electrical switchboards
---
# Switchboard critical failure

Critical failure can occur when the switch is suffering from a severe error that effects its core chips and caused them to fail. In this scenario, a hard reset cannot clear the error and there is a need to repair the failure within the core chips first in order.
In order to repair the chips, the engineer will need to open the designated junction box on the switchboard, connect the EMU device to the service port and to run the repair program. 

## The EMU repair program

The repair program will use different utilities in order to fix the faulty chips. It is up to the engineer to figure out which chips are requiring repair and which utility is needed.
The program will assist the engineer by providing feedback after every repair attempt, however each attempt is creating more load on the system and after 12 attempts the system will shut down in order to protect the chips from additional damage.  

## Utilities and chips

Each switchboard can have its own unique chips and required utilities. The EMU repair program will identify the chips in the system and will load the relevant utilities. 
