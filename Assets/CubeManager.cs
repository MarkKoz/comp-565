using System;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.EventSystems;

public class CubeManager : MonoBehaviour
{
    public UIManager ui;

    private Camera mainCamera;
    private IReadOnlyDictionary<UIManager.Texture, Material> materials;

    private void Start()
    {
        mainCamera = Camera.main;

        if (mainCamera is null)
            throw new ApplicationException("An enabled main camera was not found.");

        materials = new Dictionary<UIManager.Texture, Material>
        {
            {
                UIManager.Texture.StoneBricks01,
                Resources.Load<Material>("Materials/stone_bricks01")
            },
            {
                UIManager.Texture.StoneBricks02,
                Resources.Load<Material>("Materials/stone_bricks02")
            },
            { UIManager.Texture.Metal01, Resources.Load<Material>("Materials/metal01") },
        };
    }

    private void Update()
    {
        if (EventSystem.current.IsPointerOverGameObject()) return;

        if (Input.GetMouseButtonDown(0)) CreateCubeOnHit();
    }

    private void CreateCubeOnHit()
    {
        bool hit = Physics.Raycast(mainCamera.ScreenPointToRay(Input.mousePosition), out var hitInfo);

        if (!hit) return;

        if (hitInfo.transform.tag.Equals("Base"))
            CreatePrimitive(new Vector3(hitInfo.point.x, hitInfo.point.y + 0.5f, hitInfo.point.z));
        else
            CreatePrimitive(hitInfo.transform.position + hitInfo.normal);
    }

    private void CreatePrimitive(Vector3 position)
    {
        var primitive = GameObject.CreatePrimitive(ui.primitive);
        primitive.GetComponent<MeshRenderer>().material = materials[ui.texture];
        primitive.transform.position = position;
    }
}
