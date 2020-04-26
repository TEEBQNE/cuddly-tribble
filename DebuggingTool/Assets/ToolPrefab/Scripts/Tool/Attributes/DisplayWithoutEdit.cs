// https://answers.unity.com/questions/489942/how-to-make-a-readonly-property-in-inspector.html
// didn't feel like writing this myself, just a safetry procaution so designer's do not mess up
// the save/load functionality 

using UnityEngine;
using System;

/// <summary>
/// Allow to display an attribute in inspector without allow editing
/// </summary>
[AttributeUsage(AttributeTargets.Property | AttributeTargets.Field)]
public class DisplayWithoutEdit : PropertyAttribute { }