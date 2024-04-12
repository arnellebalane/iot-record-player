import 'dotenv-defaults/config.js';
import crypto from 'node:crypto';
import { URL, URLSearchParams } from 'node:url';
import axios from 'axios';

const spotifyAuthorizeEndpoint = 'https://accounts.spotify.com/authorize';
const spotifyTokenEndpoint = 'https://accounts.spotify.com/api/token';
const spotifyApiEndpoint = 'https://api.spotify.com/v1';
const oauthRedirectUrl = 'https://iot-rp.arnelle.dev/oauth/spotify';

const clientId = process.env.SPOTIFY_CLIENT_ID;

export function getCodeVerifier() {
    const alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    const values = crypto.getRandomValues(new Uint8Array(64));
    const verifier = values.reduce((code, x) => code + alphabet[x % alphabet.length], '');
    return verifier;
}

export async function getCodeChallenge(verifier) {
    const buffer = Buffer.from(verifier);
    const hashed = await crypto.subtle.digest('SHA-256', buffer);
    const challenge = Buffer.from(hashed).toString('base64url');
    return challenge;
}

export function getAuthorizationUrl(challenge) {
    const url = new URL(spotifyAuthorizeEndpoint);
    url.search = new URLSearchParams({
        client_id: clientId,
        response_type: 'code',
        scope: 'user-modify-playback-state',
        redirect_uri: oauthRedirectUrl,
        code_challenge: challenge,
        code_challenge_method: 'S256',
    }).toString();
    return url.toString();
}

export async function startAuthorization() {
    const verifier = getCodeVerifier();
    const challenge = await getCodeChallenge(verifier);
    const authorizationUrl = getAuthorizationUrl(challenge);
    return { verifier, challenge, authorizationUrl };
}

export async function getAccessToken(code, verifier) {
    const params = new URLSearchParams({
        client_id: clientId,
        grant_type: 'authorization_code',
        code,
        redirect_uri: oauthRedirectUrl,
        code_verifier: verifier,
    }).toString();
    const { data } = await axios.post(spotifyTokenEndpoint, params);
    return data;
}

export async function startPlayback(contextUri, accessToken) {
    const body = {
        context_uri: contextUri,
    };
    await axios.put(`${spotifyApiEndpoint}/me/player/play`, body, {
        headers: {
            'Content-Type': 'application/json',
            Authorization: `Bearer ${accessToken}`,
        },
    });
}
