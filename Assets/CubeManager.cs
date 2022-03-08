using System;

using JetBrains.Annotations;

using UnityEngine;

public class CubeManager : MonoBehaviour
{
    [CanBeNull] private Camera mainCamera;
    public UIManager ui;

    private void Start()
    {
        mainCamera = Camera.main;

        if (mainCamera is null)
            throw new ApplicationException("An enabled main camera was not found.");
    }

    private void Update()
    {
        if (Input.GetMouseButtonDown(0)) CreateCubeOnHit();
    }

    private void CreateCubeOnHit()
    {
        bool hit = Physics.Raycast(mainCamera!.ScreenPointToRay(Input.mousePosition), out var hitInfo);

        if (!hit) return;

        if (hitInfo.transform.tag.Equals("Base"))
            CreateCube(new Vector3(hitInfo.point.x, hitInfo.point.y + 0.5f, hitInfo.point.z));
        else
            CreateCube(hitInfo.transform.position + hitInfo.normal);
    }

    private static void CreateCube(Vector3 position)
    {
        var cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        cube.tag = "Cube";
        cube.transform.position = position;
    }
}
