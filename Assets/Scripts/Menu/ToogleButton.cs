using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ToogleButton : MonoBehaviour {

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
        }
    }
}
