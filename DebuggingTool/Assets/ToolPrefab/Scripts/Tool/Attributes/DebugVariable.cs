using System;

/// <summary>
/// Used to determine which variables will be debuged
/// has multiple parameters to alter what data is
/// tracked/shown to end user
/// </summary>
[AttributeUsage(AttributeTargets.Property)]
public class DebugVariable : Attribute
{
    //private bool mShouldShowCallStack;
    //private bool mShouldShowDeltaGraph;
    //private bool mEnableAutoChanges;

    // default const
    public DebugVariable() { }

    // decided against using variable overloaded constructor
    // all settings always on

    // overloaded const
    //public DebugVariable(bool showCallStack, bool showDeltaGraph, bool enableAutoChanges)
    //{
    //    mShouldShowCallStack = showCallStack;
    //    mShouldShowDeltaGraph = showDeltaGraph;
    //    mEnableAutoChanges = enableAutoChanges;
    //}
    //
    //public virtual bool ShowingCallStack
    //{
    //    get { return mShouldShowCallStack; }
    //}
    //
    //public virtual bool ShowingDeltaGraph
    //{
    //    get { return mShouldShowDeltaGraph; }
    //}
    //
    //public virtual bool EnabledAutos
    //{
    //    get { return mEnableAutoChanges; }
    //}
}