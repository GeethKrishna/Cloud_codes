#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_SIZE 3

typedef struct{
	int sequence_number;
	char data;
	bool ack_received;
} Frame;

void send_frame(Frame *frames, int start, int end){
	for (int i = start; i <= end; ++i){
		printf("Sending Frame %d: %c\n", frames[i].sequence_number, frames[i].data);
	}
}

void receive_ack(Frame *frames, int ack_number){
	frames[ack_number].ack_received = true;
	printf("Received ACK for Frame %d\n", ack_number);
}

int main(){
	Frame frames[WINDOW_SIZE];
	int total_frames = 50; // Number of frames to send
	int base = 0;
	int next_seq_num = 0;

	// Initialize frames
	for (int i = 0; i < WINDOW_SIZE && i < total_frames; ++i){
		frames[i].sequence_number = i;
		frames[i].data = 'A' + i;
		frames[i].ack_received = false;
	}

	while (base < total_frames){
		// Send frames within the window
		send_frame(frames, base, next_seq_num);

		// Simulate receiving ACKs
		for (int i = base; i <= next_seq_num; ++i){
			if (!frames[i].ack_received){
				// Simulate the transmission delay and reception of ACK
				// In a real scenario, this might involve network communication
				if (rand() % 4 == 0){
					receive_ack(frames, i);
				}
			}
		}

		// Move the window
		while (base < total_frames && frames[base].ack_received){
			++base;
		}

		// Slide the window
		int remaining_frames = total_frames - next_seq_num - 1;
		int frames_to_send = remaining_frames > 0 ? (remaining_frames < WINDOW_SIZE ? remaining_frames : WINDOW_SIZE) : 0;

		for (int i = 0; i < frames_to_send; ++i){
			++next_seq_num;
			frames[next_seq_num].sequence_number = next_seq_num;
			frames[next_seq_num].data = 'A' + next_seq_num;
			frames[next_seq_num].ack_received = false;
		}
	}

	printf("All frames sent and acknowledged.\n");

	return 0;
}
