using UnityEngine;
using UnityEngine.EventSystems;

public class Hide : MonoBehaviour {

	// Use this for initialization
	void Start ()
    {
        	
	}
    public void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.button == PointerEventData.InputButton.Left)
        {
            GetComponentInParent<ClickableObject>().HidePoly();
        }
        else if (eventData.button == PointerEventData.InputButton.Middle)
        {

        }
        else if (eventData.button == PointerEventData.InputButton.Right)
        {

        }
    }
    // Update is called once per frame
    void Update () {
		
	}
}
