using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ClickableObject : MonoBehaviour, IPointerClickHandler
{
    
    public void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.button == PointerEventData.InputButton.Left)
            Debug.Log("Left click");
        else if (eventData.button == PointerEventData.InputButton.Middle)
        {
            if (Shifts.instance.obj1 == null || Shifts.instance.obj1 == "") { return; }
            Shifts.instance.obj2 = GetComponentInChildren<Text>().text;
            int shape1 = Loader.instance.FindShape(Shifts.instance.obj1);
            int shape2 = Loader.instance.FindShape(Shifts.instance.obj2);
            Vector3 position = Loader.instance.Shapes[shape1].points[0];
            for (int i = 0; i < Loader.instance.Shapes[shape2].points.Count; i++)
            {
                Shifts.instance.Shift(Shifts.instance.obj1, Loader.instance.Shapes[shape2].points[i], 1, true);
            }
            Shifts.instance.Shift(Shifts.instance.obj1, Loader.instance.Shapes[shape2].points[0], 1, true);
            Shifts.instance.Shift(Shifts.instance.obj1, position, 1, true);
            Shifts.instance.NextTask();
        }
        else if (eventData.button == PointerEventData.InputButton.Right)
            Shifts.instance.obj1 = GetComponentInChildren<Text>().text;
    }
                
}