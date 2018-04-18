using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Play : MonoBehaviour {
    private void Start()
    {
        PlayCore(); //Temp
    }
    public void PlayCore()
    {
        Shifts.instance.pause = false;
        transform.Find("Text").GetComponent<Text>().text = "Pause";
    }
    public void StopCore()
    {
        Shifts.instance.pause = true;
        transform.Find("Text").GetComponent<Text>().text = "Play";
    }
    public void SwitchCore()
    {
        if (Shifts.instance.pause == true)
        {
            PlayCore();
        }
        else
        {
            StopCore();
        }
    }
}
