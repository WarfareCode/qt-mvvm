// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "applicationmodelsinterface.h"
#include "google_test.h"
#include "projectmanager.h"
#include "test_utils.h"
#include <cctype>
#include <mvvm/model/propertyitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/utils/fileutils.h>

namespace
{
const std::string samplemodel_name = "samplemodel";

} // namespace

//! Tests for ProjectManager class.

class ProjectManagerTest : public ::testing::Test
{
public:
    ~ProjectManagerTest();

    class ApplicationModels : public ApplicationModelsInterface
    {
    public:
        std::unique_ptr<ModelView::SessionModel> sample_model;
        ApplicationModels()
            : sample_model(std::make_unique<ModelView::SessionModel>(samplemodel_name))
        {
        }

        std::vector<ModelView::SessionModel*> persistent_models() const override
        {
            return {sample_model.get()};
        };
    };

    static inline std::string test_dir = "not-yet-defined";
    static inline const std::string test_subdir = "test_QuickReflProjectManager";
    static void SetUpTestCase() { test_dir = TestUtils::CreateTestDirectory(test_subdir); }
    std::string testDir() const { return test_dir; }

    //! Creates project directory in the test directory and returns full path.
    //! Remove recursively previous one with the same name, if exist.
    std::string create_project_dir(const std::string& name)
    {
        auto path = ModelView::Utils::join(testDir(), name);
        ModelView::Utils::remove_all(path);
        ModelView::Utils::create_directory(path);
        return path;
    }
};

ProjectManagerTest::~ProjectManagerTest() = default;

//! Initial state of ProjectManager. Project created, and not-saved.

TEST_F(ProjectManagerTest, initialState)
{
    ApplicationModels models;
    ProjectManager manager(&models);
    EXPECT_TRUE(manager.currentProjectDir().empty());
    EXPECT_FALSE(manager.isModified());
}

// ----------------------------------------------------------------------------
// Untitled, empty project
// ----------------------------------------------------------------------------

//! Creating new project. Use untitled+empty project as a starting point.
//! Should succeed, since old empty project doesn't need to be saved.

TEST_F(ProjectManagerTest, untitledEmptyNew)
{
    ApplicationModels models;
    ProjectManager manager(&models);

    const auto project_dir = create_project_dir("Project_untitledEmptyNew");
    EXPECT_TRUE(manager.createNewProject(project_dir));

    EXPECT_EQ(manager.currentProjectDir(), project_dir);
    EXPECT_FALSE(manager.isModified());

    // project directory should contain a json file with the model
    auto model_json = ModelView::Utils::join(project_dir, samplemodel_name + ".json");
    EXPECT_TRUE(ModelView::Utils::exists(model_json));
}

//! Saving of new project. Use untitled+empty project as a starting point.
//! Should fail since project directory is not defined.

TEST_F(ProjectManagerTest, untitledEmptySave)
{
    ApplicationModels models;
    ProjectManager manager(&models);
    EXPECT_FALSE(manager.saveCurrentProject());
    EXPECT_FALSE(manager.isModified());
}

//! Saving of new project. Use untitled+empty project as a starting point.
//! Should be saved, file sould appear on disk.

TEST_F(ProjectManagerTest, untitledEmptySaveAs)
{
    ApplicationModels models;
    ProjectManager manager(&models);

    const auto project_dir = create_project_dir("Project_untitledEmptySaveAs");
    EXPECT_TRUE(manager.saveProjectAs(project_dir));
    EXPECT_FALSE(manager.isModified());

    // project directory should contain a json file with the model
    auto model_json = ModelView::Utils::join(project_dir, samplemodel_name + ".json");
    EXPECT_TRUE(ModelView::Utils::exists(model_json));
}

// ----------------------------------------------------------------------------
// Untitled, modified
// ----------------------------------------------------------------------------

//! Creating new project. Use untitled+modified project as a starting point.
//! Should fail, since modified old project will prevent creation of the new one.

TEST_F(ProjectManagerTest, untitledModifiedNew)
{
    ApplicationModels models;
    ProjectManager manager(&models);

    // modifying the model
    models.sample_model->insertItem<ModelView::PropertyItem>();

    EXPECT_TRUE(manager.isModified());

    const auto project_dir = create_project_dir("Project_untitledModifiedNew");
    EXPECT_FALSE(manager.createNewProject(project_dir));

    EXPECT_TRUE(manager.currentProjectDir().empty());
    EXPECT_TRUE(manager.isModified());

    // project directory should be empty
    auto model_json = ModelView::Utils::join(project_dir, samplemodel_name + ".json");
    EXPECT_FALSE(ModelView::Utils::exists(model_json));
}

//! Saving of new project. Use untitled+modified project as a starting point.
//! Should fail since project directory is not defined.

TEST_F(ProjectManagerTest, untitledModifiedSave)
{
    ApplicationModels models;
    ProjectManager manager(&models);
    // modifying the model
    models.sample_model->insertItem<ModelView::PropertyItem>();

    EXPECT_FALSE(manager.saveCurrentProject());
    EXPECT_TRUE(manager.isModified());
}

//! Saving of new project. Use untitled+empty project as a starting point.
//! Should be saved, file sould appear on disk.

TEST_F(ProjectManagerTest, untitledModifiedSaveAs)
{
    ApplicationModels models;
    ProjectManager manager(&models);
    models.sample_model->insertItem<ModelView::PropertyItem>(); // modifying the model

    const auto project_dir = create_project_dir("Project_untitledModifiedSaveAs");
    EXPECT_TRUE(manager.saveProjectAs(project_dir));
    EXPECT_FALSE(manager.isModified());

    // project directory should contain a json file with the model
    auto model_json = ModelView::Utils::join(project_dir, samplemodel_name + ".json");
    EXPECT_TRUE(ModelView::Utils::exists(model_json));
}

// ----------------------------------------------------------------------------
// Titled, unmodified
// ----------------------------------------------------------------------------

//! Creating new project. Use titled+unmodified project as a starting point.
//! Should succeed, since old empty project doesn't need to be saved.

TEST_F(ProjectManagerTest, titledUnmodifiedNew)
{
    ApplicationModels models;
    ProjectManager manager(&models);

    const auto project_dir = create_project_dir("Project_titledUnmodifiedNew");
    EXPECT_TRUE(manager.saveProjectAs(project_dir));
    EXPECT_EQ(manager.currentProjectDir(), project_dir);

    const auto project_dir2 = create_project_dir("Project_titledUnmodifiedNew2");
    EXPECT_TRUE(manager.createNewProject(project_dir2));

    EXPECT_EQ(manager.currentProjectDir(), project_dir2);
    EXPECT_FALSE(manager.isModified());

    // project directory should contain a json file with the model
    auto model_json = ModelView::Utils::join(project_dir2, samplemodel_name + ".json");
    EXPECT_TRUE(ModelView::Utils::exists(model_json));
}


// ----------------------------------------------------------------------------
// Titled, modified
// ----------------------------------------------------------------------------

//! Saving of new project. Use titled+modified project as a starting point.
//! Should succeed.

TEST_F(ProjectManagerTest, titledModifiedSave)
{
    ApplicationModels models;
    ProjectManager manager(&models);

    const auto project_dir = create_project_dir("Project_titledModifiedSave");
    EXPECT_TRUE(manager.saveProjectAs(project_dir));
    EXPECT_EQ(manager.currentProjectDir(), project_dir);

    // modifying the model
    models.sample_model->insertItem<ModelView::PropertyItem>();

    EXPECT_TRUE(manager.saveCurrentProject());
    EXPECT_FALSE(manager.isModified());
}
