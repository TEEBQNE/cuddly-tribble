# Debbugging Final Champlain 2020

For the final project in my Debugging Class, I decided to make a Unity non nattive platform specific tool to aid in solving remote bugs. The tool can currently track any serializable data types, display the value of it in real time, display a visual call stack of every change to every variable, allow for realtime editing of any variable and display a change over time of up to 20 changes of numeric variable types on a line graph. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

```
Unity 2019.1.0f2+
```

### Installing

Firstly, make sure you have Unity installed using Unity Hub with a version over or equal to 2019.1.0f2.

```
To verify you have a proper version of Unity installed, go to Unity Hub, and on the bottom left hand of the UI selection tab, click on Installs. As long as you have a version that reads in 2019, the project should be fine to open in it. 
```

Once you verify you have the proper version, add the project to your Unity Hub.

```
Click on the top right button add, navigate to the root project folder, select it and the project should appear now. If you have the exact version of Unity I have downloaded, you can just open the project up, if not, you will need to select which version you want to open the project in. Again, I highly recommend you download the same version, or at the very least a version after the one I am using.
```

## Running the tests

Running the program is rather simple. I have set everything up with a test game object and test class. Simply hit the Unity play button, or make a build for your specific OS and run it. Once in play mode or in game, hit the escape key. 

If you would like to add the tool to track your own variable, I would recommend tracking only primitive data types as tracking other types can be a bit involved. The example project currently tracks ints, floats, strings and vec3s to show how to set everything up. When you have a variable you would like to track, make a public property for it, giving it the setter/getter methods. 

`
// test variable for the tool
[SerializeField]
[DisplayWithoutEdit]
private int testInt;

// accessor variable to get setter/getter methods
    [DebugVariable]
    public int TestInt
    {
        get => testInt;
        set
        {
            // store all data that was created here
            DebuggingManager.Instance.RecieveDebuggingData
            (
                gameObject.name + this.GetType().Name + nameof(TestInt),                    // unique ID of this variable/script/gameobject
                testInt.ToString(),                                                         // old value of variable
                value.ToString(),                                                           // new value of variable
                Time.timeSinceLevelLoad.ToString("0.0")                                    // current time that this was set for graph
            );

            // set the variable like normal
            testInt = value;
        }
    }`
    
If you follow this template and attach it to every variable, making sure that the attribute and property is set up correctly, the program will find it on startup. If you have trouble setting anything up, don't hestitate to drop a comment or contact me directly!

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Dan for an awesome class
* Coding Monkey for help with buggy UI graphs
