//
//  WorldEditorTab.cpp
//  Anima
//
//  Created by Marco Zille on 04/12/14.
//
//

#include "WorldEditorTab.h"
#include "AnimaEditorDocument.h"

WorldEditorTab::WorldEditorTab(AnimaEditorDocument* doc)
{
	_document = doc;
	
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("World editor"));
}

void WorldEditorTab::saveSettings(QSettings* settings)
{
	
}

void WorldEditorTab::readSettings(QSettings* settings)
{
	
}