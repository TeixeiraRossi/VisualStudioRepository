#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif // !WX_PRECOMP
#include <string.h>

class ticTacToe : public wxApp {
public:
	virtual bool OnInit();
};

class MainFrame : public wxFrame {
public:
	MainFrame();
protected:
	wxMenuBar* m_mainMenuBar;
	wxMenu* m_menuGame;
	wxMenu* m_menuHelp;


	wxGridSizer* m_gridSizer;
	wxButton* m_buttons[3][3];
	char currentPlayer = 'X';
private:
	void OnNewGame(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnButtonClicked(wxCommandEvent& event);
	bool checkWinner(char player);
	bool checkDraw();
	void ShowEndMessage(const wxString& message);
};


bool ticTacToe::OnInit() {
	MainFrame* frame = new MainFrame();
	frame->Show();
	//Inicializar aqui.
	return true;
}

enum {
	ID_NewGame = 1
};


MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Tic Tac Toe") {
	this->Center();
	this->SetSizeHints(600, 600);
	
	
	m_mainMenuBar = new wxMenuBar(0);
	m_menuGame = new wxMenu();
	m_menuHelp = new wxMenu();

	m_mainMenuBar->Append(m_menuGame, "Game");
	m_mainMenuBar->Append(m_menuHelp, "Help");

	m_menuGame->Append(ID_NewGame, "&New Game\tCtrl-N", "Start a new match");
	m_menuGame->AppendSeparator();
	m_menuGame->Append(wxID_EXIT);
	m_menuHelp->Append(wxID_ABOUT);

	CreateStatusBar();



	wxFont font(48, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	this->SetMenuBar(m_mainMenuBar);


	
	wxFont btnFont(36, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxPanel* panel = new wxPanel(this);	
	m_gridSizer = new wxGridSizer(3, 3, 0, 0); //3 row, 3 cols

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m_buttons[i][j] = new wxButton(panel, 1000 + i * 3 + j, "", wxDefaultPosition, wxSize(150, 150));
			m_buttons[i][j]->SetFont(btnFont); //font style
			m_gridSizer->Add(m_buttons[i][j], 1, wxEXPAND | wxALL, 5);

			//associate event of button to a function
			m_buttons[i][j]->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
		}
	}

	panel->SetSizer(m_gridSizer);
	panel->SetBackgroundColour(wxColor(240, 248, 255));//alice blue
	m_gridSizer->Layout();




	Bind(wxEVT_MENU, &MainFrame::OnNewGame, this, ID_NewGame);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);

}


void MainFrame::OnButtonClicked(wxCommandEvent& event) {
	wxButton* clickedButton = dynamic_cast<wxButton*>(event.GetEventObject());

	if (clickedButton && clickedButton->GetLabel().IsEmpty()) {
		clickedButton->SetLabel(wxString::Format("%c", currentPlayer));
		clickedButton->Enable(false); //Stop Button

		if (currentPlayer == 'X') {
			clickedButton->SetBackgroundColour(*wxBLUE);
			clickedButton->SetForegroundColour(*wxWHITE);
		}
		else {
			clickedButton->SetBackgroundColour(*wxRED);
			clickedButton->SetForegroundColour(*wxWHITE);
		}

		//call function to win
		if (checkWinner(currentPlayer)) {
			ShowEndMessage(wxString::Format("Player %c wins!", currentPlayer));
			return;
		}


		//verify if´s draw
		if (checkDraw()) {
			ShowEndMessage("IT'S A DRAW!");
			return;
		}


		//turn player
		currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';

	}
}

void MainFrame::OnNewGame(wxCommandEvent& event) {
	currentPlayer = 'X';
	SetStatusText("Player X starts!");

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m_buttons[i][j]->SetLabel("");
			m_buttons[i][j]->Enable(true);

			//call original colors
			m_buttons[i][j]->SetBackgroundColour(wxNullColour);
			m_buttons[i][j]->SetForegroundColour(wxNullColour);
		}
	}
}

bool MainFrame::checkWinner(char player) {
	//row e cols
	for (int i = 0; i < 3; i++) {
		if (m_buttons[i][0]->GetLabel() == player &&
			m_buttons[i][1]->GetLabel() == player &&
			m_buttons[i][2]->GetLabel() == player) {
			return true;
		}

		if (m_buttons[0][i]->GetLabel() == player &&
			m_buttons[1][i]->GetLabel() == player &&
			m_buttons[2][i]->GetLabel() == player) {
			return true;
		}
	}
	//diagonals
	if (m_buttons[0][0]->GetLabel() == player &&
		m_buttons[1][1]->GetLabel() == player &&
		m_buttons[2][2]->GetLabel() == player) {
		return true;
	}
	if (m_buttons[0][2]->GetLabel() == player &&
		m_buttons[1][1]->GetLabel() == player &&
		m_buttons[2][0]->GetLabel() == player) {
		return true;
	}


	return false;
}

bool MainFrame::checkDraw() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (m_buttons[i][j]->GetLabel().IsEmpty()) {
				return false;
			}
		}
	}
	
	return true;
}

void MainFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("Tic tac toe game for learning C++.", "oba", wxOK | wxICON_INFORMATION);
}

void MainFrame::ShowEndMessage(const wxString& message) {
	wxMessageBox(message, "Game Over", wxOK | wxICON_INFORMATION);
	
	//Desable buttons

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m_buttons[i][j]->Enable(false);
		}
	}
}

wxIMPLEMENT_APP(ticTacToe);