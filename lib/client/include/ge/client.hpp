#pragma once

namespace ge {

class Sprite {
};

class Client {
public:
    Client();

    bool isAlive() const;

    void processInput();
    void update(double delta);

private:
    
};

} // namespace ge
