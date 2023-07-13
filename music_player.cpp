#include <iostream>
#include <SDL2/SDL.h>
#include <mpg123.h>
#include <string>
#include <fstream>

using namespace std;

string timeline = "▋";

// Audio callback function
void AudioCallback(void* userdata, Uint8* stream, int len) {
    mpg123_handle* handle = static_cast<mpg123_handle*>(userdata);
    size_t bytesRead = 0;
    int err = mpg123_read(handle, stream, len, &bytesRead);

    if (err != MPG123_OK && err != MPG123_DONE) {
        std::cerr << "Failed to read audio: " << mpg123_strerror(handle) << std::endl;
        SDL_memset(stream, 0, len);  // Fill the stream with silence on error
    } else if (bytesRead < len) {
        SDL_memset(stream + bytesRead, 0, len - bytesRead);  // Fill the remaining stream with silence
    }
}

int main()
{
    system("clear");
    std::string filename = "song.txt";  // Replace with your file name

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }
    std::string line;
    std::string song;
    while (std::getline(file, line)) {
        // Process the line here
        song = line;
    }
    file.close();


 // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize libmpg123
    mpg123_init();
    mpg123_handle* handle = mpg123_new(nullptr, nullptr);

    int err = mpg123_open(handle, song.c_str());

    if (err != MPG123_OK) {
        std::cerr << "Failed to open audio file: " << mpg123_strerror(handle) << std::endl;
        mpg123_delete(handle);
        mpg123_exit();
        SDL_Quit();
        return 1;
    }

    // Configure audio specifications based on the MP3 file
    long rate;
    int channels, encoding;
    mpg123_getformat(handle, &rate, &channels, &encoding);
    SDL_AudioSpec spec;
    spec.freq = rate;
    spec.format = AUDIO_S16SYS;
    spec.channels = channels;
    spec.samples = 2048;
    spec.callback = AudioCallback;
    spec.userdata = handle;

    // Open the audio device
    if (SDL_OpenAudio(&spec, nullptr) < 0) {
        std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
        mpg123_close(handle);
        mpg123_delete(handle);
        mpg123_exit();
        SDL_Quit();
        return 1;
    }

    // Start audio playback
    SDL_PauseAudio(0);

    // Get total audio length
    double totalSeconds = mpg123_length(handle) / (double)rate;
    int totalMinutes = static_cast<int>(totalSeconds / 60);
    int totalSecondsRemainder = static_cast<int>(totalSeconds) % 60;
    
    // return 0;
    // Playback loop
    bool quit = false;
    int cnt = 0;
    while (!quit) {
        // Get current audio position
        double currentPosition = mpg123_tellframe(handle) / (double)rate;
        int currentMinutes = static_cast<int>(currentPosition / 60);
        int currentSecondsRemainder = (static_cast<int>(currentPosition) % 60)*100;

        // Calculate percentage
        double percentage = (currentPosition / totalSeconds) * 100000;

        std::cout << "Playing: " << song << "\n\n";
        printf("\n%.0f% : ", percentage);
        if(percentage>=cnt+1){
            timeline += "▋";
            cnt++;
        }
        cout<<timeline; cout<<"\n\n";
        // cout<<timeline<<"\n\n";
        std::cout << "'q' -> quit | 'ENTER' -> update" << std::endl;
        if (std::cin.get() == 'q') quit = true;
        system("clear");
    }

    // Stop audio playback
    SDL_PauseAudio(1);

    // Close the audio device
    SDL_CloseAudio();

    // Clean up libmpg123
    mpg123_close(handle);
    mpg123_delete(handle);
    mpg123_exit();

    // Quit SDL
    SDL_Quit();

    return 0;
}