using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ToogleButton : MonoBehaviour {
    private void Start()
    {
        int index = Loader.instance.FindShape(GetComponentInChildren<Text>().text);
        if (index < 0) { return; }
        if (Loader.instance.Shapes[index].gameObject == null) { return; }
        ColorBlock cb = GetComponent<Button>().colors;
        cb.normalColor = Loader.instance.Shapes[index].gameObject.GetComponentInChildren<RunTimeCreator>().fillColor;
        Color newColor = Loader.instance.Shapes[index].gameObject.GetComponentInChildren<RunTimeCreator>().fillColor;
        newColor.g += 0.5f;
        cb.highlightedColor = newColor;
        GetComponent<Button>().colors = cb;
    }
    public void TooglePoly()
    {
        int index = Loader.instance.FindShape(GetComponentInChildren<Text>().text);
        if (index < 0) { return; }
        if (Loader.instance.Shapes[index].gameObject == null) { return; }
        if (Loader.instance.Shapes[index].gameObject.activeSelf)
        {
            Loader.instance.Shapes[index].gameObject.SetActive(false);
        }
        else
        {
            Loader.instance.Shapes[index].gameObject.SetActive(true);
            Loader.instance.SetActiveObject(Loader.instance.Shapes[index]);
        }
    }
}
