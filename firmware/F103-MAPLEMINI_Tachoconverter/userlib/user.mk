USERLIB = ./userlib
# List of all the Userlib files

USERSRC =  $(USERLIB)/src/myStringfunctions.c  \
           $(USERLIB)/src/eeprom.c \
           $(USERLIB)/src/comm.c \
           $(USERLIB)/src/table.c \
           $(USERLIB)/src/i2c_local.c
            
# Required include directories
USERINC =  $(USERLIB) \
           $(USERLIB)/include 
