# Prerequisite

Unreal Engine 4.23

Visual Studio 2017 or 2019 with C++ tools and Unreal Engine Installer modules Installed

https://docs.unrealengine.com/en-US/Programming/Development/VisualStudioSetup/index.html


# How to Open the project

Before you try to compile the project make sure that you have the right version of the socket.io plugin in the plugins folder for the unreal engine version you are using (1.4.0 for UE 4.25 and 1.3.0 for UE 4.24)

You can find all the find all the versions of the socket.io plugin at https://github.com/getnamo/socketio-client-ue4/releases



Right click on the .uproject and and generate the visual studio project files

Compile the project under visual studio

Open the project with unreal engine


## Heroku integration
Once you have your Heroku app created, you will need to change the url in the AlexaSocket.cpp file at line 17 to point to your Heroku server url

# Key bindings

WASD to move

E to interact with items on the floor

Q to open the inventory

1234 to use an ability



# Folder Structure

## Source

Contain all C++ classes.



## Content

Contain all Blueprints classes and Assets.

The project's Blueprint classes are in the AlexaRPG directory.

The other directories are assets libraries.
