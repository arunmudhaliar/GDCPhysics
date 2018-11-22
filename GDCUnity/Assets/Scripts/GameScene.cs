using System.Collections;
using System.Collections.Generic;
using Photon.Pun;
using UnityEngine;
using UnityEngine.UI;

public class GameScene : MonoBehaviour {

    bool roomJoined = false;

    [Tooltip("The prefab to use for representing the player")]
    [SerializeField]
    private GameObject playerPrefab = null;

    [Tooltip("The prefab to use for representing the ball")]
    [SerializeField]
    private GameObject ballPrefab = null;

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
            if (PhotonNetwork.IsMasterClient) {
                GameObject masterPlayer = PhotonNetwork.Instantiate(this.playerPrefab.name, new Vector3(-6.8f, 0f, 0f), Quaternion.identity, 0);
                masterPlayer.name = "masterPlayer";
                GameObject masterBall = PhotonNetwork.Instantiate(this.ballPrefab.name, new Vector3(-2.0f, 3f, 0f), Quaternion.identity, 0);
                masterBall.name = "masterBall";
                masterPlayer.GetComponent<Player>().myBall = masterBall;
            } else {
                GameObject clientPlayer = PhotonNetwork.Instantiate(this.playerPrefab.name, new Vector3(6.8f, 0f, 0f), Quaternion.identity, 0);
                clientPlayer.name = "clientPlayer";
                GameObject clientBall = PhotonNetwork.Instantiate(this.ballPrefab.name, new Vector3(2.0f, 3f, 0f), Quaternion.identity, 0);
                clientBall.name = "clientBall";
                clientPlayer.GetComponent<Player>().myBall = clientBall;
            }
        }


        //if (Ball.localBall == null) {
        //    Debug.LogFormat("We are Instantiating LocalPlayer from {0}", SceneManagerHelper.ActiveSceneName);

        //    // we're in a room. spawn a character for the local player. it gets synced by using PhotonNetwork.Instantiate
        //    if (PhotonNetwork.IsMasterClient) {
        //        GameObject masterPlayer = PhotonNetwork.Instantiate(this.playerPrefab.name, new Vector3(-6.0f, 0f, 0f), Quaternion.identity, 0);
        //        masterPlayer.name = "masterPlayer";
        //        GameObject masterBall = PhotonNetwork.Instantiate(this.ballPrefab.name, new Vector3(-4.0f, 0f, 0f), Quaternion.identity, 0);
        //        masterBall.name = "masterBall";
        //        masterPlayer.GetComponent<Player>().myBall = masterBall;
        //    } else {
        //        GameObject clientPlayer = PhotonNetwork.Instantiate(this.playerPrefab.name, new Vector3(6f, 0f, 0f), Quaternion.identity, 0);
        //        clientPlayer.name = "clientPlayer";
        //        GameObject clientBall = PhotonNetwork.Instantiate(this.ballPrefab.name, new Vector3(4.0f, 0f, 0f), Quaternion.identity, 0);
        //        clientBall.name = "clientBall";
        //        clientPlayer.GetComponent<Player>().myBall = clientBall;
        //    }
        //}
    }
}
