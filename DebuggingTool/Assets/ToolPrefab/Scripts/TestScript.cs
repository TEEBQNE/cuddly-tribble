using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestScript : MonoBehaviour
{
    // test variable for the tool
    [SerializeField]
    [DisplayWithoutEdit]
    private int testInt;

    // test float
    [SerializeField]
    [DisplayWithoutEdit]
    private float testFloat;

    // test vector3
    [SerializeField]
    [DisplayWithoutEdit]
    private Vector3 testVec3;

    // test string
    [SerializeField]
    [DisplayWithoutEdit]
    private string testString;

    // add a test for a bool
    // add a test for a char
    // add a test for a double

    // accessor variable to get setter/getter methods
    [DebugVariable]
    public Vector3 TestVec3
    {
        get => testVec3;
        set
        {
            // store all data that was created here
            DebuggingManager.Instance.RecieveDebuggingData
            (
                gameObject.name + this.GetType().Name + nameof(TestVec3),                   // unique ID of this variable/script/gameobject
                testVec3.ToString(),                                                        // old value of variable
                value.ToString(),                                                           // new value of variable
                Time.timeSinceLevelLoad.ToString("0.0")                                    // current time that this was set for graph
            );

            // set the variable like normal
            testVec3 = value;
        }
    }

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
    }

    [DebugVariable]
    public string TestString
    {
        get => testString;
        set
        {
            // store all data that was created here
            DebuggingManager.Instance.RecieveDebuggingData
            (
                gameObject.name + this.GetType().Name + nameof(TestString),                 // unique ID of this variable/script/gameobject
                testString,                                                                 // old value of variable
                value,                                                                      // new value of variable
                Time.timeSinceLevelLoad.ToString("0.0")                                    // current time that this was set for graph
            );

            // set the variable like normal
            testString = value;
        }
    }

    [DebugVariable]
    public float TestFloat
    {
        get => testFloat;
        set
        {
            // store all data that was created here
            DebuggingManager.Instance.RecieveDebuggingData
            (
                gameObject.name + this.GetType().Name + nameof(TestFloat),                  // unique ID of this variable/script/gameobject
                TestFloat.ToString(),                                                       // old value of variable
                value.ToString(),                                                           // new value of variable
                Time.timeSinceLevelLoad.ToString("0.0")                                    // current time that this was set for graph
            );

            // set the variable like normal
            testFloat = value;
        }
    }

    void OnGUI()
    {

        Event e = Event.current;

        //Check the type of the current event, making sure to take in only the KeyDown of the keystroke.
        //char.IsLetter to filter out all other KeyCodes besides alphabetical.
        if (e.type == EventType.KeyDown && e.keyCode.ToString().Length == 1 && char.IsLetter(e.keyCode.ToString()[0]))
        {
            TestString += e.keyCode;
        }
    }

        // Update is called once per frame
    void Update()
    {
        TestFloat++;

        if(Input.GetKey(KeyCode.Space))
        {
            IncreaseTheVec3();
        }

        RandomFunction();
    }

    void IncreaseTheVec3()
    {
        RaiseTheY();
    }

    void RaiseTheY()
    {
        DoSomethingToZ();
    }

    void DoSomethingToZ()
    {
        TestVec3 = new Vector3(Random.Range(-100, 250), Random.Range(-100, 250), Random.Range(-100, 250));
    }


    void RandomFunction()
    {
        if (Input.GetKeyDown(KeyCode.B))
        {
            TestInt--;
        }
    }
}

// below was a failed attempt at store a delegate to the setter method but it is not a method
// Field access isn't performed via a method (like getters and setters)--it's performed with an IL
// instruction--so there's nothing I can assign to a delegate. I would have to use the expression
// route to create a "block" of code (effectively IL) that can be assigned to a delegate.

// I left this here only because it took my hours to figure out (I made something similar myself but it also
// did not work for the same reason that the snippet from stack overflow fails)

// used for the test that failed
// using UnityEngine.UI;
// using System;
// using System.Reflection;
// using System.Reflection.Emit;
// using System.Globalization;

// I do not want to do this right now - not enough time
// below is what it could look like, but this implementation creates a new setter/getter method
// I could override the getter/setter methods such as https://stackoverflow.com/questions/82437/why-is-it-impossible-to-override-a-getter-only-property-and-add-a-setter
// but I am not going to do this as it would make every single getter/setter method call my debugging manager
// where I would only want the ones with the attribute to call it.
// If I could find a way to attach a listener to the existing setter method, that would be ideal, but I can't due to it not being a method :\
///// <summary>
///// https://stackoverflow.com/questions/16073091/is-there-a-way-to-create-a-delegate-to-get-and-set-values-for-a-fieldinfo
///// </summary>
///// <typeparam name="S"></typeparam>
///// <typeparam name="T"></typeparam>
///// <param name="field"></param>
///// <returns></returns>
//static Func<S, T> CreateGetter<S, T>(FieldInfo field)
//{
//    string methodName = field.ReflectedType.FullName + ".get_" + field.Name;
//    DynamicMethod setterMethod = new DynamicMethod(methodName, typeof(T), new Type[1] { typeof(S) }, true);
//    ILGenerator gen = setterMethod.GetILGenerator();
//    if (field.IsStatic)
//    {
//        gen.Emit(OpCodes.Ldsfld, field);
//    }
//    else
//    {
//        gen.Emit(OpCodes.Ldarg_0);
//        gen.Emit(OpCodes.Ldfld, field);
//    }
//    gen.Emit(OpCodes.Ret);
//    return (Func<S, T>)setterMethod.CreateDelegate(typeof(Func<S, T>));
//}
//
///// <summary>
///// https://stackoverflow.com/questions/16073091/is-there-a-way-to-create-a-delegate-to-get-and-set-values-for-a-fieldinfo
///// </summary>
///// <typeparam name="S"></typeparam>
///// <typeparam name="T"></typeparam>
///// <param name="field"></param>
///// <returns></returns>
//static Action<S, T> CreateSetter<S, T>(FieldInfo field)
//{
//    string methodName = field.ReflectedType.FullName + ".set_" + field.Name;
//    DynamicMethod setterMethod = new DynamicMethod(methodName, null, new Type[2] { typeof(S), typeof(T) }, true);
//    ILGenerator gen = setterMethod.GetILGenerator();
//    if (field.IsStatic)
//    {
//        gen.Emit(OpCodes.Ldarg_1);
//        gen.Emit(OpCodes.Stsfld, field);
//    }
//    else
//    {
//        gen.Emit(OpCodes.Ldarg_0);
//        gen.Emit(OpCodes.Ldarg_1);
//        gen.Emit(OpCodes.Stfld, field);
//    }
//    gen.Emit(OpCodes.Ret);
//    return (Action<S, T>)setterMethod.CreateDelegate(typeof(Action<S, T>));
//}
