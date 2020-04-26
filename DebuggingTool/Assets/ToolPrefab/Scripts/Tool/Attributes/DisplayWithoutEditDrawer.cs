// https://answers.unity.com/questions/489942/how-to-make-a-readonly-property-in-inspector.html
// could have rewritten this, simple enough just need base functionality so designer's can't break
// save load

#if UNITY_EDITOR

using UnityEngine;
using UnityEditor;

[CustomPropertyDrawer(typeof(DisplayWithoutEdit))]
public class DisplayWithoutEditDrawer : PropertyDrawer
{


    /// <summary>
    /// Display attribute and his value in inspector depending on the type
    /// Fill attribute needed
    /// </summary>
    public override float GetPropertyHeight(SerializedProperty property,
                                            GUIContent label)
    {
        return EditorGUI.GetPropertyHeight(property, label, true);
    }

    public override void OnGUI(Rect position,
                               SerializedProperty property,
                               GUIContent label)
    {
        GUI.enabled = false;
        EditorGUI.PropertyField(position, property, label, true);
        GUI.enabled = true;
    }
}
#endif