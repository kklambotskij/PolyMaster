using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Reset : MonoBehaviour {

	// Use this for initialization
	void Start ()
    {
        ResetCamera();
	}
    public void ResetCamera()
    {
        Camera.main.transform.position = new Vector3(0, 0, 0);
    }
}
