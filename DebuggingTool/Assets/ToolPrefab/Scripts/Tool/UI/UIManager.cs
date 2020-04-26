using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using System.Linq;

public class UIManager : MonoBehaviour
{
    //_________________Singleton_________________//

    // singleton instance of the event manager
    private static UIManager uiManager;

    //____________________UI_____________________//

    // prefab reference to a new button
    public GameObject newButton;

    // reference to where the buttons need to be added to
    public Transform variableButtonScrollHolder;

    // reference to the current variable text
    public TextMeshProUGUI currentVarText;

    // reference to the call stack index text
    public TextMeshProUGUI currentstackIndexText;

    // reference to the current value text
    public TextMeshProUGUI currentValue;

    // reference to the UI to be generated for the visual call stack
    public GameObject stackObjectRef;

    // reference to the stack holder
    public GameObject stackHolder;

    // canvas groups to turn on/off all UI for the tool
    public CanvasGroup canvasReferences;

    // reference to the graph script
    public Window_Graph graphWindowRef;

    // reference to UI when graph can not be drawn
    public TextMeshProUGUI graphFailText;

    //___________________ObjRef__________________//

    // reference to the current viewed object
    VariableDebuggingInfo localInfo = null;

    // reference to current callstack dictionary string
    [HideInInspector]
    public string callStackRefString = "";

    // current index of the callstack being viewed
    int currentViewingCallStackIndex;

    // current number of calls in the current stack
    int currentNumberOfStackCalls;

    // determines if the UI is shown
    bool isOn;

    // reference to all generated blocks for the stack holder
    List<TextMeshProUGUI> UICallStackRef = new List<TextMeshProUGUI>();

    // grabs the instance of the event manager
    /// <summary>
    ///
    /// Singleton instance to the ui manager (Fix the singelton to delete other instances if they exist)
    /// 
    /// </summary>
    public static UIManager Instance
    {
        // getter method
        get
        {
            // if no event manager exists, then find it
            if (!uiManager)
            {
                uiManager = FindObjectOfType(typeof(UIManager)) as UIManager;

                // if it can not find it, then specify that an object in the scene needs to have it
                if (!uiManager)
                {
                    Debug.Log("There needs to be one active EventManager script on a gameobject in your scene.");
                }
                else
                {
                    // if not, then init the event manager
                    uiManager.Init();

                }
            }
            return uiManager;
        }
    }

    /// <summary>
    ///
    /// Initialize the event dictionary if it does not exist
    /// 
    /// </summary>
    void Init()
    {
        // can't init the data until call stack data is used
        //localnfo = DebuggingManager.Instance.GetInfoByName("");
    }

    /// <summary>
    /// Creates a new button giving the name and adds the button onclick
    /// </summary>
    /// <param name="buttonName"></param>
    public void SpawnNewButton(string buttonName)
    {
        GameObject tempBt = Instantiate(newButton);
        tempBt.transform.SetParent(variableButtonScrollHolder, false);
        tempBt.GetComponentInChildren<TextMeshProUGUI>().text = buttonName;
        tempBt.GetComponent<Button>().onClick.AddListener(() => DebuggingManager.Instance.VariableButtonClick(buttonName));
    }

    /// <summary>
    /// Sets the current variable name
    /// </summary>
    /// <param name="variableName"></param>
    public void SetNewCurrentVariable(string variableName)
    {
        currentVarText.text = variableName;
    }

    public void SetCurrentValue(string value)
    {
        currentValue.text = "Current Value is: " + value;
    }

    public void ChangeCallStack(int index)
    {
        if (localInfo == null)
        {
            currentstackIndexText.text = "No Stack Data";
            return;
        }

        int tempNum = currentViewingCallStackIndex;
        currentViewingCallStackIndex += index;

        if (currentViewingCallStackIndex > localInfo.stackData.Count-1)
            currentViewingCallStackIndex = 0;

        if (currentViewingCallStackIndex < 0)
            currentViewingCallStackIndex = localInfo.stackData.Count - 1;

        // don't redraw UI if there is no need (wasteful)
        if(tempNum != currentViewingCallStackIndex)
            UpdateCallStackData(currentViewingCallStackIndex);
    }

    /// <summary>
    /// Redraws all of parts of the debugging UI
    /// </summary>
    /// <param name="info">Current new info</param>
    /// <param name="unchangedVar">Determines if the variable has been changed or is just updating current variable</param>
    public void UpdateCurrentUI(VariableDebuggingInfo info, bool unchangedVar, string currentName)
    {
        // update local info
        localInfo = info;

        // update graph - currently not efficient
        // in the window_graph.cs, there is a comment
        // on ho to make it efficent - currently went with
        // quick and dirty as ran out of time with this project
        UpdateGraphData();

        // set the current variable name
        SetNewCurrentVariable(currentName);

        // if local info is null, set the callstack buttons to non interactable
        if (localInfo == null)
        {
            ChangeCallStack(-1);
            SetCurrentValue("");
            WipeStack();
            currentValue.text = "Current Value: ";
            return;
        }

        // set the current value of this variable
        SetCurrentValue(localInfo.currentValue);

        // draw the updated data for the graph
        // if it is unchanged, just remove first index
        // and add a new last depending on size of list
        // or just add a new index

        // update the callstack list (possibly might not need a redraw?)
        // just do a check based on the current callstack variable stored
        // if it is different, then it needs an update
        if (unchangedVar)
        {
            // if the callstack is unset, then set it here
            if (callStackRefString == "")
            {
                UpdateCallStackData(0);
            }
            else
            {
                if(callStackRefString == localInfo.currentStackString)
                    UpdateCallStackData(currentViewingCallStackIndex);
            }
        }
        else
        {
            // 100% needs a redraw, the list changed
            // display the first index in the dictionary
            UpdateCallStackData(0);
        }
    }

    public void ClearInputField(TMP_InputField fieldText)
    {
        fieldText.text = "";
    }

    /// <summary>
    /// Updates the current graph data if the data being read is of some numerical type
    /// I could extend the graphing to heat maps for vec2/vec3, (2D and 3D graphs)
    /// and a web graph or flow for character/string values
    /// Other data such as list, structs, etc. could also be stored and displayed, but that would be not optimal
    /// </summary>
    public void UpdateGraphData()
    {
        // has valid data, if not, then hide the objects
        if(localInfo != null && localInfo.graphData.Count != 0)
        {
            if (graphWindowRef.CanDisplay(localInfo.graphData) == 1)
            {
                // clear text as graph displayed correctly
                graphFailText.text = "";
            }
            else
            {
                // display error text
                graphFailText.text = "Current datatype not supported!";
            }
        }
        else
        {
            // clear the list, no data to display
            graphWindowRef.DestroyList();

            graphFailText.text = "No Data to Display";
        }
    }

    public void WipeStack()
    {
        currentstackIndexText.text = "No Stack Data";
        currentNumberOfStackCalls = 0;
        foreach (TextMeshProUGUI textObj in UICallStackRef)
        {
            textObj.transform.parent.gameObject.SetActive(false);
        }
    }

    public void UpdateCallStackData(int index)
    {
        if (localInfo == null)
        {
            currentstackIndexText.text = "No Stack Data";
            return;
        }
        currentstackIndexText.text = "CallStack Index: " + index;
        // inefficient but stack should never exceed 10+ values
        // this does a linear search of a dictionary which is BAD
        // but it's max maybe 10 stack calls for each variable
        string data = localInfo.stackData.ElementAt(index).Key;

        callStackRefString = data;

        currentViewingCallStackIndex = index;

        // break the string into it's components by the delimeter
        string[] actualData = data.Split('|');

        int fixedLength = actualData.Length - 1;
        int UIStackSize = UICallStackRef.Count;
        // keep a list of prefab instances of the callstack blocks
        // if this value is less than the current stack, then spawn new blocks
        // then set all blocks in order
        // if this value is greater than the current stack, then set all current
        // block data, then disbale the blocks that are unused
        // if this value is equal to, just set all the data
        if(fixedLength > currentNumberOfStackCalls)
        {
            if (fixedLength > UIStackSize)
            {
                // spawn new blocks
                for (int x = 0; x < fixedLength - UIStackSize; ++x)
                {
                    // instantiate a new stack object
                    GameObject temp = Instantiate(stackObjectRef);
                    temp.transform.SetParent(stackHolder.transform, false);
                    UICallStackRef.Add(temp.transform.GetChild(0).GetComponent<TextMeshProUGUI>());
                }
            }
            else
            {
                for(int x = 0; x < fixedLength; ++x)
                {
                    UICallStackRef[x].transform.parent.gameObject.SetActive(true);
                }
            }
               
        }
        else if(fixedLength < UICallStackRef.Count)
        {
            // disbale all objects that will not be used by current stack
            for(int x = fixedLength; x < UIStackSize; x++)
            {
                UICallStackRef[x].transform.parent.gameObject.SetActive(false);
            }
        }

        // just set all of the data here now, we have the proper number of blocks that are
        // also rendering visibility
        for (int x = 0; x < fixedLength-1; ++x)
        {
            UICallStackRef[x].transform.parent.gameObject.SetActive(true);
            UICallStackRef[x].text = actualData[x];
        }

        currentNumberOfStackCalls = fixedLength;

        // display the last call that actually changed the value
        UICallStackRef[fixedLength-1].text = actualData[fixedLength-1] + " " + localInfo.stackData[data];
    }

    /// <summary>
    /// Toggles all UI for the end user
    /// </summary>
    public void ToggleUI()
    {
        // toggle bool
        isOn = !isOn;

        // disable or enable the UI
        canvasReferences.alpha = isOn ? 1 : 0;
        canvasReferences.interactable = isOn;
        canvasReferences.blocksRaycasts = isOn;
    }
}