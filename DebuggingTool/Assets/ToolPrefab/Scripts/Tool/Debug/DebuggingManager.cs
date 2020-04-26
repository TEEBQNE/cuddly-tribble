using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Reflection;
using System;
using System.Diagnostics;
using System.Linq;

/// <summary>
/// Holds data for each variables changed graph data over time
/// </summary>
public struct GraphDataStruct
{
    public string data;
    public string timestamp;
}

/// <summary>
/// Class to hold all debugging info by variable name
/// </summary>
public class VariableDebuggingInfo
{
    // current value of data
    public string currentValue;

    // old value
    public string oldValue;

    // current time since ran - used in the graph data
    public string currentTime; 

    // unique individual stack data
    // key = stackName+stackName+...+stackName
    public Dictionary<string, string> stackData = new Dictionary<string,string>();

    // graph data for timestamps and relavant data
    // make sure if it is the first index, to store the old and new data
    public List<GraphDataStruct> graphData = new List<GraphDataStruct>();

    // current stack variable to update UI
    public string currentStackString;
}

/// <summary>
/// Used to callback fieldinfo based on unique ID of
/// gameobject + mono name + variable name
/// </summary>
public struct CallBackData
{
    public PropertyInfo fi;
    public MonoBehaviour mono;
}

/// <summary>
/// Class to grab all references to debugging variables
/// then store information for the UI and tracking
/// </summary>
public class DebuggingManager : MonoBehaviour
{
    // dictionary of every variable being tracked
    Dictionary<string, VariableDebuggingInfo> variableData = new Dictionary<string, VariableDebuggingInfo>();

    // list of all callbacks for when a user tries to change data on the fly
    //List<CallBackData> callbackList = new List<CallBackData>();
    Dictionary<string, CallBackData> callbackDict = new Dictionary<string, CallBackData>();

    public int MAX_GRAPH_DATA = 20;

    // assuming the user has at least 1 debug variable
    // can add error handling later
    string currentVariableViewing = "";

    // determines if this is inited or not
    private bool init;

    // singleton instance of the event manager
    private static DebuggingManager debugManager;

   /// <summary>
   /// Just a singleton
   /// </summary>
    public static DebuggingManager Instance
    {
        // getter method
        get
        {
            // if no debug manager exists, then find it
            if (!debugManager)
            {
                debugManager = FindObjectOfType(typeof(DebuggingManager)) as DebuggingManager;

                // if it can not find it, then specify that an object in the scene needs to have it
                if (!debugManager)
                {
                    UnityEngine.Debug.Log("There needs to be one active DebugManager script on a gameobject in your scene.");
                }
                else
                {
                    // if not, then init the event manager
                    if(!debugManager.init)
                        debugManager.Init();

                }
            }
            return debugManager;
        }
    }

    private void Start()
    {
        if(!init)
        {
            init = true;
            Init();
        }
    }

    /// <summary>
    ///
    /// Initialize the event dictionary if it does not exist
    /// 
    /// </summary>
    void Init()
    {
        // find all objects in the scene
        MonoBehaviour[] sceneActive = FindObjectsOfType<MonoBehaviour>();

        // loop through each object
        foreach (MonoBehaviour mono in sceneActive)
        {
            // grab all of the properties on all monobehaviours in the scene
            PropertyInfo[] objectProperties = mono.GetType().GetProperties(BindingFlags.Instance | BindingFlags.Public | BindingFlags.Static);

            // iterate over all of the data found on this script
            for (int i = 0; i < objectProperties.Length; i++)
            {
                DebugVariable debugging = Attribute.GetCustomAttribute(objectProperties[i], typeof(DebugVariable)) as DebugVariable;
                if (debugging != null)
                {
                    // found an attribute match, extract data
                    objectProperties[i].GetValue(mono);

                    // list structs that hold a button name GameObject + Script + Variable
                    // add an onclick with the index of where this data is in the list
                    // the struct also holds the fieldinfo reference
                    // when a button is clicked, it sets all the UI to the key of the button name
                    // then get the fieldInfo from the list to use setvalue on the variable
                    string uniqueID = mono.name + mono.GetType().Name + objectProperties[i].Name;

                    // create a button, assign it's onclick to the index that the list is currently set to
                    // so the data is tethered to the button
                    UIManager.Instance.SpawnNewButton(uniqueID);

                    // add the callback to the list for later use
                    callbackDict.Add(uniqueID, new CallBackData { fi = objectProperties[i], mono = mono });
                }
            }
        }

        // errors will throw if this is not deactivated with 0 debug variables
        if (callbackDict.Count == 0)
            gameObject.SetActive(false);

        // set default settings to first index of button
        // if this fails, throw an error as there needs to be at least 1 object
        currentVariableViewing = callbackDict.Keys.ElementAt(0);

        UIManager.Instance.SetCurrentValue(callbackDict[currentVariableViewing].fi.GetValue(callbackDict[currentVariableViewing].mono).ToString());

        // update the UI for which variable is being viewed
        UIManager.Instance.UpdateCurrentUI(null, true, currentVariableViewing);
    }

    /// <summary>
    /// Returns the info for the referenced variable data
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    public VariableDebuggingInfo GetInfoByName(string name)
    {
        // init - default to first value in list
        if (name == "")
            return variableData.Values.ElementAt(0);
        else
            return variableData[name];
    }

    /// <summary>
    /// Determines which index in the list has been clicked
    /// no data is set until a user enters and confirms data in the
    /// entry form box, this is just to link which variable is active
    /// </summary>
    public void VariableButtonClick(string variableName)
    {
        currentVariableViewing = variableName;
        UIManager.Instance.callStackRefString = "";
        UIManager.Instance.UpdateCurrentUI(variableData.ContainsKey(currentVariableViewing) ? variableData[currentVariableViewing] : null, false, currentVariableViewing);
    }

    private void Update()
    {
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            UIManager.Instance.ToggleUI();
        }
    }

    /// <summary>
    /// Recieves all debugging data to store from the setter method of the variable
    /// </summary>
    public void RecieveDebuggingData(string variableID, string oldValue, string newValue,  string currentTime)
    {
        // reference to the debugging data of the current variable
        VariableDebuggingInfo localDebugger = null;

        // get data about the stack trace (can call this in the 
        StackTrace stackTrace = new StackTrace();
        StackFrame[] frames = stackTrace.GetFrames();

        // id of the stack trace
        string localStackTrace = "";

        // build the string to see if we even need this data
        for (int x = frames.Length - 1; x > 1; --x)
        {
            //concatinate the current stack and add the delimeter
            localStackTrace += frames[x].GetMethod().Name + "|";
        }

        // if no variable data exists, then create the new variable
        if(!variableData.ContainsKey(variableID))
        {
            variableData.Add(variableID, new VariableDebuggingInfo());
        }

        // grab the reference to the current variable info
        localDebugger = variableData[variableID];

        // _______________________ STACK DATA _____________________ //
        // if no info exists for this stack trace, then just add it
        if(!localDebugger.stackData.ContainsKey(localStackTrace))
        {
            // add whatever data you want to display as the value
            // key is the actual stack trace

            // maybe store the iteration count of this particular stack??

            // add in the data
            localDebugger.stackData.Add(localStackTrace, oldValue + " => " + newValue);
        }
        else
        {
            // this stack exists, just update data or handle it differently?
            // could keep an interative counter on each stack to tell user how many times
            // this has been called
            // also possibly check if the value on this stack has changed
            // if not, ignore the data

            // only update the data if it is NEW data
            if(oldValue != newValue)
            {
                localDebugger.stackData[localStackTrace] = oldValue + " => " + newValue;
            }
        }

        // ________________________________________________________ //


        // _______________________ GRAPH DATA _____________________ //
        // add the graph data (Keep a max on this data and only add it if the data
        // has changed (hardoc
        // if the current length is 0, just add the first two values on change
        if (localDebugger.graphData.Count == 0)
        {
            // add the old value
            // timetsmp set to 0 as it is initialzed at whatever the older value was
            localDebugger.graphData.Add(new GraphDataStruct { data = oldValue, timestamp = "0" });
        }
        else if(localDebugger.graphData.Count == MAX_GRAPH_DATA)
        {
            // remove the first index then add normally
            localDebugger.graphData.RemoveAt(0);
        }

        // add a new graph struct
        localDebugger.graphData.Add(new GraphDataStruct { data = newValue, timestamp = currentTime });
        // ________________________________________________________ //

        // _________________ SINGLE VARIABLE DATA _________________ //

        // set the current time and value to be displayed
        localDebugger.currentValue = newValue;
        localDebugger.currentTime = currentTime;
        localDebugger.oldValue = oldValue;
        localDebugger.currentStackString = localStackTrace;

        // send an update to the UI with the variable ID
        // variable is unique ID

        // if the current data passed in is passed to the currently viewed variable
        // then update the UI
        if (currentVariableViewing == variableID)
        {
            // could move each section to it's own canvas to avoid isDirty if only sections are changed
            // need to ALWAYS update the graph for now
            UIManager.Instance.UpdateCurrentUI(localDebugger, true, currentVariableViewing);
        }
    }

    /// <summary>
    /// Function can only be called if all data is frozen
    /// setting the variable specified by index and fails by error
    /// if it is unable to set it
    /// </summary>
    /// <param name="data"></param>
    public void ChangeData(string data)
    {
        // do a try catch here as what the user typed in may
        // not work - can not validate as not sure what they have entered
        // in the catch condition, throw an error or display that the value entered
        // can not be converted to the type of the variable they are setting
        try
        {
            callbackDict[currentVariableViewing].fi.SetValue(callbackDict[currentVariableViewing].mono, System.Convert.ChangeType(data, callbackDict[currentVariableViewing].fi.PropertyType));
        }
        catch
        {
            UnityEngine.Debug.LogError("Error: data entered did not match the type of the variable selected. The variables type is: " + callbackDict[currentVariableViewing].fi.PropertyType.ToString() + ". Please try again and make sure to format the data to match!");
        }
    }
}