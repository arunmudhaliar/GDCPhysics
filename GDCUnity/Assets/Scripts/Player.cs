using System.Collections;
using System.Collections.Generic;
using Photon.Pun;
using UnityEngine;

public class Player : MonoBehaviourPunCallbacks, IPunObservable
{
    [Tooltip("The local player instance. Use this to know if the local player is represented in the Scene")]
    public static GameObject localPlayer;
    public GameObject myBall;

    public void Awake()
    {
        // #Important
        // used in GameManager.cs: we keep track of the localPlayer instance to prevent instanciation when levels are synchronized
        if (photonView.IsMine)
        {
            localPlayer = gameObject;
        }

        // #Critical
        // we flag as don't destroy on load so that instance survives level synchronization, thus giving a seamless experience when levels load.
        DontDestroyOnLoad(gameObject);
    }

    void Update() {
        if (photonView.IsMine) {
            float speed = 10.0f;
            if (Input.GetKey(KeyCode.UpArrow)) {
                if (transform.position.y < 3.0f) {
                    transform.position += Vector3.up * speed * Time.deltaTime;
                } else {
                    transform.position = new Vector3(transform.position.x, 3.0f, 0.0f);
                }
            }
            if (Input.GetKey(KeyCode.DownArrow)) {
                transform.position += Vector3.up * -speed * Time.deltaTime;
                if (transform.position.y > -3.0f) {
                    transform.position += Vector3.up * -speed * Time.deltaTime;
                } else {
                    transform.position = new Vector3(transform.position.x, -3.0f, 0.0f);
                }
            }
        }
    }

    public void OnPhotonSerializeView(PhotonStream stream, PhotonMessageInfo info)
    {
        if (stream.IsWriting)
        {
            // We own this player: send the others our data
            //stream.SendNext(this.IsFiring);
            //stream.SendNext(this.Health);
        }
        else
        {
            // Network player, receive data
            //this.IsFiring = (bool)stream.ReceiveNext();
            //this.Health = (float)stream.ReceiveNext();
        }
    }

    void OnCollisionEnter2D( Collision2D collision ) {
        //Debug.Log("Collided "+ collision.gameObject.tag);
        if (collision.gameObject.tag == "ball" && photonView.IsMine) {
            if (collision.contacts.Length>0) {
                Vector2 collisionNormal = Vector2.zero;
                collisionNormal = -collision.contacts[0].normal;
                collision.rigidbody.AddForce(collisionNormal * 6.0f, ForceMode2D.Impulse);
            }
        }
    }
}
