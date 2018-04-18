using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Sebastian.Geometry
{
   [System.Serializable]
    public class Shape
    {
        public List<Vector3> points;
        public string name;
        public GameObject gameObject;
        private Shape shape;

        public Shape()
        {
            points = new List<Vector3>();
        }
        public Shape(List<Vector3> pts)
        {
            points = new List<Vector3>(pts);
        }
        public Shape(List<Vector3> pts, string name)
        {
            points = new List<Vector3>(pts);
            this.name = name;
            gameObject = GameObject.Find(name);
            if (gameObject != null) { Debug.Log(name + " poly was succesfully loaded"); }
            else { Debug.Log(name + " poly load ERROR"); }
        }
        public Shape(string name)
        {
            points = new List<Vector3>();
            this.name = name;
            gameObject = GameObject.Find(name);
            if (gameObject != null) { Debug.Log(name + " poly was succesfully loaded"); }
            else { Debug.Log(name + " poly load ERROR"); }
        }
        public Shape(Shape shape)
        {
            gameObject = shape.gameObject;
            name = shape.name;
            if (gameObject == null)
            {
                gameObject = GameObject.Find(name);
                Debug.Log(name + " poly was succesfully loaded");
            }
            else
            {
                Debug.Log(name + " poly load ERROR");
            }
            points = new List<Vector3>(shape.points);
        }
    }
}