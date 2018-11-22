using System.Collections;
using System.Collections.Generic;
using Photon.Pun;
using UnityEngine;
using UnityEngine.UI;

public class GameScene : MonoBehaviour {

    bool roomJoined = false;

    [Tooltip("The prefab to use for representing the player")]
    [SerializeField]
    private GameObject playerPrefab;

    private void OnGUI()
    {
        if (PhotonNetwork.CurrentRoom != null)
        {
            var roomname = PhotonNetwork.CurrentRoom.Name;
            if (roomname == "") {
                Debug.LogError("Room name empty");
            }
            GUILayout.Label("Joined to room " + roomname);
            roomJoined = true;
        }
        else
        {
            GUILayout.Label("No room joined");
        }

    }

    private void Update()
    {
        if (!roomJoined) {
            return;
        }

        if (Player.localPlayer == null)
        {
            Debug.LogFormat("We are Instantiating LocalPlayer from {0}", SceneManagerHelper.ActiveSceneName);

            // we're in a room. spawn a character for the local player. it gets synced by using PhotonNetwork.Instantiate
            PhotonNetwork.Instantiate(this.playerPrefab.name, new Vector3(0f, 5f, 0f), Quaternion.identity, 0);
        }
    }
}
