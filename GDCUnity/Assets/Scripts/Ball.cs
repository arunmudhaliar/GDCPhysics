using System.Collections;
using System.Collections.Generic;
using Photon.Pun;
using UnityEngine;

public class Ball : MonoBehaviourPunCallbacks, IPunObservable {
    [Tooltip("The local ball instance. Use this to know if the local ball is represented in the Scene")]
    public static GameObject localBall;

    public void Awake() {
        // #Important
        // used in GameManager.cs: we keep track of the localPlayer instance to prevent instanciation when levels are synchronized
        if (photonView.IsMine) {
            localBall = gameObject;
        } else {
            GetComponent<Rigidbody2D>().isKinematic = true;
            GetComponent<BoxCollider2D>().isTrigger = true;
        }

        // #Critical
        // we flag as don't destroy on load so that instance survives level synchronization, thus giving a seamless experience when levels load.
        DontDestroyOnLoad(gameObject);
    }

    public void OnPhotonSerializeView( PhotonStream stream, PhotonMessageInfo info ) {
        if (stream.IsWriting) {
            // We own this player: send the others our data
            //stream.SendNext(this.IsFiring);
            //stream.SendNext(this.Health);
        } else {
            // Network player, receive data
            //this.IsFiring = (bool)stream.ReceiveNext();
            //this.Health = (float)stream.ReceiveNext();
        }
    }
}
