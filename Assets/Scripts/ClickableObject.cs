using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ClickableObject : MonoBehaviour, IPointerClickHandler
{
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
    public void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.button == PointerEventData.InputButton.Left)
            TooglePoly();
        else if (eventData.button == PointerEventData.InputButton.Middle)
        {
            if (Shifts.instance.obj1 == null || Shifts.instance.obj1 == "") { return; }
            Shifts.instance.obj2 = GetComponentInChildren<Text>().text;
            int shape1 = Loader.instance.FindShape(Shifts.instance.obj1);
            int shape2 = Loader.instance.FindShape(Shifts.instance.obj2);
            if (shape1 < 0 || shape1 >= Loader.instance.Shapes.Count) { return; }
            if (shape2 < 0 || shape2 >= Loader.instance.Shapes.Count) { return; }
            Vector3 position = Loader.instance.Shapes[shape1].points[0];
            for (int i = 0; i < Loader.instance.Shapes[shape2].points.Count; i++)
            {
                Shifts.instance.Shift(Shifts.instance.obj1, Loader.instance.Shapes[shape2].points[i], 1, true);
            }
            Shifts.instance.Shift(Shifts.instance.obj1, Loader.instance.Shapes[shape2].points[0], 1, true);
            Shifts.instance.Shift(Shifts.instance.obj1, position, 1, true);
            Shifts.instance.marked = Loader.instance.Shapes[shape2].points[0];
            Shifts.instance.NextTask();
        }
        else if (eventData.button == PointerEventData.InputButton.Right)
            Shifts.instance.obj1 = GetComponentInChildren<Text>().text;
    }
                
}