using System;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.EventSystems;

public class CubeManager : MonoBehaviour
{
    public UIManager ui;

    private Camera mainCamera;
    private GameObject activeGuide;
    private IReadOnlyDictionary<PrimitiveType, GameObject> guides;
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

        guides = new Dictionary<PrimitiveType, GameObject>
        {
            { PrimitiveType.Cube, transform.GetChild(0).gameObject },
            { PrimitiveType.Sphere, transform.GetChild(1).gameObject },
            { PrimitiveType.Capsule, transform.GetChild(2).gameObject },
        };
        activeGuide = guides[ui.primitive];
    }

    private void Update()
    {
        if (EventSystem.current.IsPointerOverGameObject()) return;

        bool hit = Physics.Raycast(mainCamera.ScreenPointToRay(Input.mousePosition), out var hitInfo);

        if (!hit) return;

        if (Input.GetMouseButtonDown(0))
        {
            CreatePrimitive(hitInfo);
        }
        else if (Input.GetMouseButtonDown(1))
        {
            if (!hitInfo.transform.tag.Equals("Base"))
                Destroy(hitInfo.transform.gameObject);
        }
        else
        {
            ShowGuide(hitInfo);
        }
    }

    private void ShowGuide(RaycastHit hitInfo)
    {
        var newGuide = guides[ui.primitive];

        var colour = hitInfo.transform.tag.Equals("Base") ? Color.yellow : Color.green;
        colour.a = 0.5f;
        newGuide.GetComponent<MeshRenderer>().material.color = colour;

        SetPosition(newGuide, hitInfo);

        activeGuide.SetActive(false);
        activeGuide = newGuide;
        activeGuide.SetActive(true);
    }

    private void CreatePrimitive(RaycastHit hitInfo)
    {
        var primitive = GameObject.CreatePrimitive(ui.primitive);
        primitive.GetComponent<MeshRenderer>().material = materials[ui.texture];
        SetPosition(primitive, hitInfo);
    }

    private static void SetPosition(GameObject obj, RaycastHit hitInfo)
    {
        if (hitInfo.transform.tag.Equals("Base"))
            obj.transform.position = new Vector3(hitInfo.point.x, hitInfo.point.y + 0.5f, hitInfo.point.z);
        else
            obj.transform.position = hitInfo.transform.position + hitInfo.normal;
    }
}
