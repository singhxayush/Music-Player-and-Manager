#include <bits/stdc++.h>
using namespace std;

//---------------------------------Reading .mp3 files--------------------------------

vector<pair<string, string>> songs_dir;
int number_of_songs = 0;

void find_mp3()
{
    system("find . -type f -name \"*.mp3\" -printf \"%h/%f\n\" > list.txt");

    std::string filename = "list.txt";
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        string s;
        for (int i = line.size() - 1; i >= 1; i--)
            if (line[i] == '/')
                break;
            else
                s += line[i];
        reverse(s.begin(), s.end());
        songs_dir.push_back({s, line});
        number_of_songs++;
    }

    sort(songs_dir.begin(), songs_dir.end());
    file.close();
}

void list_all_songs()
{
    system("clear");
    cout << "Favourite Songs :\n";

    for (int i = 0; i < songs_dir.size(); i++)
        cout << i + 1 << ". " << songs_dir[i].first << "\n";
    cout << songs_dir.size() + 1 << ". Back\n\n";
    // for(int i=0; i<songs_dir.size(); i++) cout<<i+1<<" "<<songs_dir[i].first<<"\t"<<songs_dir[i].second<<"\n";
    printf("\nENTER above given options to play a song or go back : ");

    int song_number;
    cin >> song_number;

    if (song_number == songs_dir.size() + 1)
        return;

    cout << songs_dir[song_number - 1].second << '\n';

    string temp;
    temp = "echo " + songs_dir[song_number - 1].second + "> song.txt";

    system(temp.c_str());
    system("clear");
    system("./music_player");
}

void starting_options()
{
    std::cout << "Options :\n\n";
    cout << "1. List All songs\n";
    cout << "2. Mark Favourite\n";
    cout << "3. Play from Favourites\n";
    cout << "4. Manage Favourites\n";
    cout << "5. Quit\n";
    cout << "\nEnter your choice : ";
}

//---------------------------------Auth System---------------------------------

string current_user;
bool auth_success = false;

struct User
{
    std::string username;
    std::string password;
};

// Function to read users from the file
std::vector<User> readUsersFromFile(const std::string &filename)
{
    std::vector<User> users;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return users;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::string username, password;
        size_t separatorPos = line.find(':');
        if (separatorPos != std::string::npos)
        {
            username = line.substr(0, separatorPos);
            password = line.substr(separatorPos + 1);
            users.push_back({username, password});
        }
    }

    file.close();
    return users;
}

// Function to write users to the file
void writeUsersToFile(const std::string &filename, const std::vector<User> &users)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for (const User &user : users)
    {
        file << user.username << ":" << user.password << "\n";
    }

    file.close();
}

// Function to check if a user exists in the list
bool userExists(const std::vector<User> &users, const std::string &username)
{
    for (const User &user : users)
    {
        if (user.username == username)
        {
            return true;
        }
    }
    return false;
}

// Function to create a new user account
void createUser(const std::string &filename)
{
    std::vector<User> users = readUsersFromFile(filename);

    std::string username, password;
    std::cout << "\nEnter username: ";
    std::cin >> username;

    if (userExists(users, username))
    {
        std::cout << "Username already exists. Please choose a different username.\n";
        return;
    }

    std::cout << "Enter password: ";
    std::cin >> password;

    users.push_back({username, password});
    writeUsersToFile(filename, users);

    std::cout << "User account created successfully!\n";

    current_user = username;
}

// Function to authenticate a user
bool authenticateUser(const std::vector<User> &users, const std::string &username, const std::string &password)
{
    for (const User &user : users)
    {
        if (user.username == username && user.password == password)
        {
            return true;
        }
    }
    return false;
}

// Function to log in
void login(const std::string &filename)
{
    std::vector<User> users = readUsersFromFile(filename);

    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;

    std::cout << "Enter password: ";
    std::cin >> password;

    if (authenticateUser(users, username, password))
    {
        std::cout << "Login successful!\n";
        auth_success = true;
        current_user = username;
    }
    else
    {
        std::cout << "Invalid username or password.\n";
    }
}

void auth()
{
    std::string filename = "users.txt";

    while (true)
    {
        if (auth_success)
            return;

        std::this_thread::sleep_for(std::chrono::seconds(2));
        system("clear");

        std::cout << "Welcome to the Music Player & Management System!\n\n";
        std::cout << "Options :\n\n";
        std::cout << "1. Login\n";
        std::cout << "2. Signup\n";
        std::cout << "3. Exit\n\n";
        int choice;
        cout << "Enter : ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            login(filename);
            break;

        case 2:
            createUser(filename);
            break;

        case 3:
            cout << "Exiting the program.\n";
            return;

        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

//-------------------------------FAV SECTION-----------------------------------

// MARK FAV
vector<pair<string, string>> fav_songs;

void mark_fav()
{
    string temp;

    if (current_user != "")
        temp = "touch " + current_user + ".txt";
    system(temp.c_str());

    system("clear");
    cout << "All Songs :\n";

    for (int i = 0; i < songs_dir.size(); i++)
        cout << i + 1 << ". " << songs_dir[i].first << "\n";
    cout << songs_dir.size() + 1 << ". Back\n\n";
    // for(int i=0; i<songs_dir.size(); i++) cout<<i+1<<" "<<songs_dir[i].first<<"\t"<<songs_dir[i].second<<"\n";
    printf("\nEnter song number to mark as favourite : ");

    int song_number;
    cin >> song_number;

    if (song_number == songs_dir.size() + 1)
        return;

    cout << songs_dir[song_number - 1].second << '\n';

    temp = "echo " + songs_dir[song_number - 1].second + ">>" + current_user + ".txt";

    system(temp.c_str());
}

// PLAY FAV
void play_fav()
{
    system("clear");

    std::string filename = current_user + ".txt"; // Replace with your file name
    std::ifstream file(filename);

    if (fav_songs.size() == 0)
    {
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        std::string line;

        while (std::getline(file, line))
        {
            string s;
            for (int i = line.size() - 1; i >= 1; i--)
                if (line[i] == '/')
                    break;
                else
                    s += line[i];
            reverse(s.begin(), s.end());
            fav_songs.push_back({s, line});
        }

        file.close();
    }

    if (fav_songs.size() == 0)
    {
        cout << "No favourite marked\n";
        return;
    }

    cout << "Favourite Songs :\n\n";

    for (int i = 0; i < fav_songs.size(); i++)
        cout << i + 1 << ". " << fav_songs[i].first << "\n";
    cout << fav_songs.size() + 1 << ". Back\n\n";
    printf("\nENTER above given options to play a song or go back : ");

    int song_number;
    cin >> song_number;

    if (song_number == fav_songs.size() + 1)
        return;

    string temp;
    temp = "echo " + fav_songs[song_number - 1].second + "> song.txt";

    system(temp.c_str());
    system("clear");
    system("./music_player");
}

// DELETE FROM FAV
void del()
{
    system("clear");
    string filename = current_user + ".txt";

    std::ifstream file(filename);

    if (fav_songs.size() == 0)
    {
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        std::string line;

        while (std::getline(file, line))
        {
            string s;
            for (int i = line.size() - 1; i >= 1; i--)
                if (line[i] == '/')
                    break;
                else
                    s += line[i];
            reverse(s.begin(), s.end());
            fav_songs.push_back({s, line});
        }

        file.close();
    }

    if (fav_songs.size() == 0)
    {
        cout << "No favourite marked\n";
        return;
    }

    cout << "Favourite Songs :\n\n";

    for (int i = 0; i < fav_songs.size(); i++)
        cout << i + 1 << ". " << fav_songs[i].first << "\n";
    cout << fav_songs.size() + 1 << ". Back\n\n";
    // for(int i=0; i<fav_songs.size(); i++) cout<<i+1<<" "<<fav_songs[i].first<<"\t"<<fav_songs[i].second<<"\n";
    printf("\nENTER above given options to Delete a song or go back : ");

    int lineToDelete;
    cin >> lineToDelete;

    if (lineToDelete == fav_songs.size() + 1)
        return;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    int lineNumber = 0;

    // Read the lines from the file and store them in a vector
    while (std::getline(inputFile, line))
    {
        ++lineNumber;
        if (lineNumber != lineToDelete)
        {
            lines.push_back(line);
        }
    }

    inputFile.close();

    std::ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Write the lines back to the file, excluding the deleted line
    for (const std::string &line : lines)
    {
        outputFile << line << std::endl;
    }

    outputFile.close();
}

//---------------------------------Main Function---------------------------------

int main()
{
    find_mp3();

    auth();
    if (!auth_success)
        return 0;

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        system("clear");

        starting_options();
        int ch;
        cin >> ch;
        switch (ch)
        {
        case 1:
            // list all songs
            list_all_songs();
            break;

        case 2:
            // mark fav
            mark_fav();
            break;

        case 3:
            // Play from fav
            play_fav();
            break;

        case 4:
            // Del from fav
            del();
            fav_songs.clear();
            break;

        case 5:
            cout << "exit\n";
            return 0;
            break;
        }
    }
}