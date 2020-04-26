using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using CodeMonkey.Utils;
using TMPro;
using System.Linq;

// a lot of this was inspired by the code monkeys graph tutorial
// was going to make my own implementation but running out of time sooooo yea
public class Window_Graph : MonoBehaviour
{
    private RectTransform graphContainer;
    public Sprite newImage;
    private RectTransform labelTemplateX;
    private RectTransform labelTemplateY;
    private RectTransform dashTemplateX;
    private RectTransform dashTemplateY;

    private List<GameObject> graphObjectsList = new List<GameObject>();

    private void Awake()
    {
        graphContainer = transform.Find("GraphContainer").GetComponent<RectTransform>();
        labelTemplateX = graphContainer.Find("LabelTemplate_X").GetComponent<RectTransform>();
        labelTemplateY = graphContainer.Find("LabelTemplate_Y").GetComponent<RectTransform>();
        dashTemplateX = graphContainer.Find("dashTemplate_Y").GetComponent<RectTransform>();
        dashTemplateY = graphContainer.Find("dashTemplate_X").GetComponent<RectTransform>();
    }

    private GameObject CreateCircle(Vector2 anchoredPosition)
    {
        GameObject tempObj = new GameObject("circle", typeof(Image));
        tempObj.transform.SetParent(graphContainer, false);
        tempObj.GetComponent<Image>().sprite = newImage;

        RectTransform rectTransform = tempObj.GetComponent<RectTransform>();
        rectTransform.anchoredPosition = anchoredPosition;
        rectTransform.sizeDelta = new Vector2(11, 11);
        rectTransform.anchorMin = new Vector2(0, 0);
        rectTransform.anchorMax = new Vector2(0, 0);

        return tempObj;
    }

    /// <summary>
    /// error handling for when a new variable button is clicked
    /// returns 1 on success, -1 on fail
    /// </summary>
    /// <param name="data"></param>
    /// <returns></returns>
    public int CanDisplay(List<GraphDataStruct> data)
    {
        DestroyList();

        try
        {
            float.Parse(data[0].data);
            ShowGraph(data);
            return 1;
        }
        catch
        {
            // invalid type, destroy the graph but display error text
            return -1;
        }
    }

    public void DestroyList()
    {
        foreach(GameObject obj in graphObjectsList)
        {
            Destroy(obj);
        }

        graphObjectsList.Clear();
    }

    private void ShowGraph(List<GraphDataStruct> data)
    {
        foreach(GameObject objRef in graphObjectsList)
        {
            Destroy(objRef);
        }

        graphObjectsList.Clear();

        float graphHeight = graphContainer.sizeDelta.y;

        // distance between each point on xAxis
        float xSize = 25f;

        // top of our graph
        float yMaximum = float.Parse(data[0].data);
        float yMinimum = float.Parse(data[0].data);

        // find min and max values
        yMaximum = data.Max(w => float.Parse(w.data));
        yMinimum = data.Min(w => float.Parse(w.data));

        yMaximum = yMaximum + ((yMaximum - yMinimum) * 0.2f);

        yMinimum = yMinimum - ((yMaximum - yMinimum) * 0.2f);

        GameObject lastCirceObj = null;
        for(int x = 0; x < data.Count; ++x)
        {
            float xPosition = 30f + x * xSize;
            float yPosition = ((float.Parse(data[x].data) - yMinimum) / (yMaximum - yMinimum)) * graphHeight;
            GameObject circleObj = CreateCircle(new Vector2(xPosition, yPosition));

            graphObjectsList.Add(circleObj);

            if (lastCirceObj != null)
            {
               GameObject dotConnection = CreateConnection(lastCirceObj.GetComponent<RectTransform>().anchoredPosition, circleObj.GetComponent<RectTransform>().anchoredPosition);
                graphObjectsList.Add(dotConnection);
            }

            lastCirceObj = circleObj;

            RectTransform labelX = Instantiate(labelTemplateX);
            labelX.SetParent(graphContainer, false);
            labelX.gameObject.SetActive(true);
            labelX.anchoredPosition = new Vector2(xPosition-10f, -20f);
            labelX.GetComponent<TextMeshProUGUI>().text = data[x].timestamp;
            graphObjectsList.Add(labelX.gameObject);

            RectTransform dashX = Instantiate(dashTemplateX);
            dashX.SetParent(graphContainer, false);
            dashX.gameObject.SetActive(true);
            dashX.anchoredPosition = new Vector2(xPosition, 0f);
            graphObjectsList.Add(dashX.gameObject);

        }

        // number of Y axis counts
        int seperatorCount = 10;

        for(int x = 0; x <= seperatorCount; ++x)
        {
            RectTransform labelY = Instantiate(labelTemplateY);
            labelY.SetParent(graphContainer, false);
            labelY.gameObject.SetActive(true);
            float normalizedValue = x * 1f/ seperatorCount;
            labelY.anchoredPosition = new Vector2(-30f, normalizedValue * graphHeight - 5f);
            labelY.GetComponent<TextMeshProUGUI>().text = Mathf.RoundToInt(yMinimum + (normalizedValue * (yMaximum - yMinimum))).ToString();
            graphObjectsList.Add(labelY.gameObject);

            RectTransform dashY = Instantiate(dashTemplateY);
            dashY.SetParent(graphContainer, false);
            dashY.gameObject.SetActive(true);
            dashY.anchoredPosition = new Vector2(0f, normalizedValue * graphHeight);
            graphObjectsList.Add(dashY.gameObject);
        }
    }

    // store the maximum height of y (delta from 0) for all values to properly scale the rest of the graph

    // only update the entire graph on a new load of a variable

    // on live update, if at max count of vars (20), then shift over all variables anchor X by xSize
    // see if a rescale in the Y is needed if new value's Y is larger than max, see if max is no longer in list
    // if max is no longer in list (is 0th element), will need to find new max and recalculate

    // once an element in 0th index is pushed off, delete the 1st index line connection and add a new
    // connection to the end node


    private GameObject CreateConnection(Vector2 dotPositionA, Vector2 dotPositionB)
    {
        GameObject tempObj = new GameObject("dotConnection", typeof(Image));
        tempObj.transform.SetParent(graphContainer, false);
        tempObj.GetComponent<Image>().color = new Color(1, 1, 1, .5f);
        Vector2 dir = (dotPositionB - dotPositionA).normalized;
        float distance = Vector2.Distance(dotPositionA, dotPositionB);
        RectTransform rectTransform = tempObj.GetComponent<RectTransform>();

        rectTransform.anchorMin = new Vector2(0, 0);
        rectTransform.anchorMax = new Vector2(0, 0);
        rectTransform.sizeDelta = new Vector2(distance, 3f);
        rectTransform.anchoredPosition = dotPositionA + dir * distance * .5f;

        rectTransform.localEulerAngles = new Vector3(0, 0, UtilsClass.GetAngleFromVector(dir));

        return tempObj;
    }
}