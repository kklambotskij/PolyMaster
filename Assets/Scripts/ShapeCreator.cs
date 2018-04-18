using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Sebastian.Geometry;

public class ShapeCreator : MonoBehaviour
{
    public MeshFilter meshFilter;

    [HideInInspector]
    public List<Shape> shapes = new List<Shape>();

    [HideInInspector]
    public bool showShapesList;

    public float handleRadius = .5f;

    public void Start()
    {

    }
    public void AddShape(Shape shape)
    {
        shapes.Add(new Shape(shape));
    }
    public void UpdateMeshDisplay()
    {
        //if (shapes.Count < 1) { return; }
        //if (shapes[0].points == null) { return; }
        //if (shapes[0].points.Count < 1) { return; }
        CompositeShape compShape = new CompositeShape(shapes);
        meshFilter.mesh = compShape.GetMesh();
    }

}