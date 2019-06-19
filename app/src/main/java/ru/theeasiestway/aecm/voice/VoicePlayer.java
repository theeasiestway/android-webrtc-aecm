package ru.theeasiestway.aecm.voice;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

public class VoicePlayer {

    private AudioTrack audioTrack;

    private void createAudioTrack(int sampleRate) {
        int minBufferSize = AudioTrack.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_CONFIGURATION_MONO, AudioFormat.ENCODING_PCM_16BIT);
        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, AudioFormat.CHANNEL_CONFIGURATION_MONO, AudioFormat.ENCODING_PCM_16BIT, minBufferSize, AudioTrack.MODE_STREAM);
    }

    public void start(int sampleRate) {
        createAudioTrack(sampleRate);
        if (audioTrack.getState() == AudioTrack.STATE_INITIALIZED) audioTrack.play();
        else {
            for (int i = 0; i < 3; i++) {
                createAudioTrack(sampleRate);
                if (audioTrack.getState() == AudioTrack.STATE_INITIALIZED) {
                    audioTrack.play();
                    break;
                }
            }
        }
    }

    public void write(short[] frame) {
        if (frame == null) return;
        audioTrack.write(frame, 0, frame.length);
    }

    public void stopPlaying() {
        if (audioTrack.getState() == AudioTrack.STATE_INITIALIZED) audioTrack.stop();
        audioTrack.flush();
    }
}