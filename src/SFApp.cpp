#include "SFApp.h"


SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), points(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 22);
  player->SetPosition(player_pos);

  
  const int number_of_aliens = 10;
  for(int i=0; i<number_of_aliens; i++) {
    // place an alien at width/number_of_aliens * i
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2(((canvas_w/number_of_aliens) * i)+25, 350.0f);
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }

  const int number_of_walls = 5;
  for(int i=0; i<number_of_walls; i++) {
    // place an wall at width/number_of_walls * i
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2(((canvas_w/number_of_walls) * i)+60, 150.0f);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }

  const int number_of_coins = 1;
  for(int i=0; i<number_of_coins; i++) {
    // place a coin at width/number_of_coins * i
    auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
    auto pos   = Point2(((canvas_w/number_of_coins) * i)+325, 450.0f);
    coin->SetPosition(pos);
    coins.push_back(coin);
  }

}

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;

  case SFEVENT_PLAYER_UP:
    player->GoNorth();

    for(auto w : walls) {
      for(auto c : coins) {
	for(auto a : aliens) {

      if(player->CollidesWith(w)) {
	 player->GoSouth();
	} else if (player->CollidesWith(c)) {

	 std::wstring s1(L"You Win");
	 std::wcout << s1 << std::endl;

	is_running = false;
	} else if (player->CollidesWith(a)) {

	 std::wstring s2(L"You Lose");
	 std::wcout << s2 << std::endl;
	 is_running = false;
	}
	}
    }}
 
    break;
    
  case SFEVENT_PLAYER_DOWN:
    player->GoSouth();

    for(auto w : walls) {
      for(auto c : coins) {
	for(auto a : aliens) {

      if(player->CollidesWith(w)) {
	 player->GoNorth();
	} else if (player->CollidesWith(c)) {

	 std::wstring s1(L"You Win");
	 std::wcout << s1 << std::endl;

	is_running = false;
	} else if (player->CollidesWith(a)) {

	 std::wstring s2(L"You Lose");
	 std::wcout << s2 << std::endl;
	 is_running = false;
	}
	}
    }}
 
    break;
    
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();

    for(auto w : walls) {
      for(auto c : coins) {
	for(auto a : aliens) {

      if(player->CollidesWith(w)) {
	 player->GoEast();
	} else if (player->CollidesWith(c)) {

	 std::wstring s1(L"You Win");
	 std::wcout << s1 << std::endl;

	is_running = false;
	} else if (player->CollidesWith(a)) {

	 std::wstring s2(L"You Lose");
	 std::wcout << s2 << std::endl;
	 is_running = false;
	}
	}
    }}
 
    break;
    
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    
      for(auto w : walls) {
      for(auto c : coins) {
	for(auto a : aliens) {

      if(player->CollidesWith(w)) {
	 player->GoWest();
	} else if (player->CollidesWith(c)) {

	 std::wstring s1(L"You Win");
	 std::wcout << s1 << std::endl;

	is_running = false;
	} else if (player->CollidesWith(a)) {

	 std::wstring s2(L"You Lose");
	 std::wcout << s2 << std::endl;
	 is_running = false;


	}
	}
    }}
 
    break;

// Win/Lose Message seems to be printed a large number of times, but not always the same amount
// I think this may have to do with the collision.
// I needed to abandon a score/points related victory as it seemed that the game was detecting
// multiple hits per projectile, 1 bullet was hitting 10+ times, as it passed through an object coin/alien
// this meant that my scoring threshold for victory was being met in 1-2 shots.
// as the number of times that each target was hit seemed to vary
// i could not use a work around of just increasing the score limit, as sometimes this limit would/would not
// be reached. As such, i opted to make the victory condition simply to collect a coin.
// it appears that the player is comming into contact with the victory coin, many times before the game closes.

   
  case SFEVENT_FIRE:
    fire ++;
    FireProjectile();
    break;

// Unable to figure this out I haven't left myself enough time to have a proper go at it.

/*  case SFEVENT_MOVE1:
  for(auto a : aliens) {
  a->Move1();
  break;
}
  case SFEVENT_MOVE2:
  for(auto a : aliens) { 
  a->Move2();
  break;
}

*/

  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {

int i;  
  
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }

  // Update enemy positions
//Unable to figure this out as above.
/*  for(auto a : aliens) {
	a->Move1();
	a->Move2(); 
}
*/
  // Detect collisions
	
// While possible to remove projectiles when hitting a wall, it seems they only stop being drawn
// but still exist, just invisibly. 'popped' projectiles seem to still kill targets some seconds later.

  for(auto p : projectiles) {
    for(auto a : aliens) {
      for(auto w : walls) {
	
	  
        if(p->CollidesWith(a)) {
         p->HandleCollision();
         a->HandleCollision();
	 
      } else if(p->CollidesWith(w)) {
	 p->HandleCollision();

      } 
}
}
}

  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp.push_back(a);

    }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);

  // remove dead coins
  list<shared_ptr<SFAsset>> tmp2;
  for(auto c : coins) {
    if(c->IsAlive()) {
      tmp2.push_back(c);
    }
  }
  coins.clear();
  coins = list<shared_ptr<SFAsset>>(tmp2);

}

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

  // draw the player
  player->OnRender();

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }

  for(auto w: walls) {
    if(w->IsAlive()) {w->OnRender();}
  }

  for(auto c: coins) {
    c->OnRender();
  }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb); 
}
