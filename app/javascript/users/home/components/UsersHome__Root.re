[@bs.config {jsx: 3}];

[%bs.raw {|require("courses/shared/background_patterns.css")|}];
[%bs.raw {|require("./UserHome__Root.css")|}];

open UsersHome__Types;

let str = React.string;

type view =
  | ShowCourses
  | ShowCommunities;

let headerSectiom = (userName, userTitle, avatarUrl, showUserEdit) => {
  <div
    className="max-w-4xl mx-auto pt-12 flex items-center justify-between px-3 lg:px-0">
    <div className="flex">
      {switch (avatarUrl) {
       | Some(src) => <img className="w-12 h-12 rounded-full mr-4" src />
       | None => <Avatar name=userName className="w-12 h-12 mr-4" />
       }}
      <div className="text-sm flex flex-col justify-center">
        <div className="text-black font-bold inline-block">
          {userName |> str}
        </div>
        <div className="text-gray-600 inline-block"> {userTitle |> str} </div>
      </div>
    </div>
    {showUserEdit
       ? <a className="btn" href="/user/edit">
           <i className="fas fa-edit text-xs md:text-sm mr-2" />
           <span> {"Edit Profile" |> str} </span>
         </a>
       : React.null}
  </div>;
};

let navButtonClasses = selected => {
  "font-semibold border-b-2 border-transparent text-sm py-4 mr-6 focus:outline-none "
  ++ (selected ? "text-primary-500 border-primary-500" : "");
};

let navSection = (view, setView) => {
  <div className="border-b mt-6">
    <div className="flex max-w-4xl mx-auto px-3 lg:px-0">
      <button
        className={navButtonClasses(view == ShowCourses)}
        onClick={_ => setView(_ => ShowCourses)}>
        <i className="fas fa-book text-xs md:text-sm mr-2" />
        <span> {"My Courses" |> str} </span>
      </button>
      <button
        className={navButtonClasses(view == ShowCommunities)}
        onClick={_ => setView(_ => ShowCommunities)}>
        <i className="fas fa-users text-xs md:text-sm mr-2" />
        <span> {"Communities" |> str} </span>
      </button>
    </div>
  </div>;
};

let courseLink = (courseId, title, suffix, icon) => {
  <a
    key=suffix
    href={"/courses/" ++ courseId ++ "/" ++ suffix}
    className="px-2 py-1 mr-2 mt-2 rounded text-sm bg-gray-100 text-gray-800 hover:bg-gray-200 hover:text-primary-500">
    <i className=icon />
    <span className="font-semibold ml-2"> {title |> str} </span>
  </a>;
};

let ctaButton = (title, suffix, courseId) => {
  <a
    href={"/courses/" ++ courseId ++ "/" ++ suffix}
    className="w-full bg-gray-200 mt-4 px-6 py-4 flex text-sm font-semibold justify-between items-center cursor-pointer text-primary-500 hover:bg-gray-300">
    <span>
      <i className="fas fa-book" />
      <span className="ml-2"> {title |> str} </span>
    </span>
    <i className="fas fa-arrow-right" />
  </a>;
};

let ctaText = message => {
  <div
    className="w-full bg-gray-200 mt-4 px-6 text-primary-500 font-semibold flex justify-between items-center">
    <span>
      <i className="fas fa-book" />
      <span className="ml-2"> {message |> str} </span>
    </span>
  </div>;
};

let callToAction = (course, currentSchoolAdmin) => {
  let courseId = course |> Course.id;

  <div>
    {switch (
       currentSchoolAdmin,
       course |> Course.review,
       course |> Course.exited,
       course |> Course.ended,
     ) {
     | (true, _, _, _) => ctaButton("View Course", "curriculum", courseId)
     | (false, true, _, _) =>
       ctaButton("Review Submissions", "review", courseId)
     | (false, false, true, _) => ctaText("Dropped out")
     | (false, false, false, true) => ctaText("Course Ended")
     | (false, false, false, false) =>
       ctaButton("Continue Course", "curriculum", courseId)
     }}
  </div>;
};

let communityLinks = (communityIds, communities) => {
  communityIds
  |> Array.map(id => {
       let community =
         communities |> Js.Array.find(c => c |> Community.id == id);
       switch (community) {
       | Some(c) =>
         <a
           key={c |> Community.id}
           href={"/communities/" ++ (c |> Community.id)}
           className="px-2 py-1 mr-2 mt-2 rounded text-sm bg-gray-100 text-gray-800 hover:bg-gray-200 hover:text-primary-500">
           <i className="fas fa-users" />
           <span className="font-semibold ml-2">
             {c |> Community.name |> str}
           </span>
         </a>
       | None => React.null
       };
     })
  |> React.array;
};

let courseLinks = (course, communities) => {
  let courseId = course |> Course.id;
  <div className="flex flex-wrap px-4 mt-2">
    {courseLink(courseId, "Curriculum", "curriculum", "fas fa-book")}
    {course |> Course.enableLeaderboard
       ? courseLink(
           courseId,
           "Leaderboard",
           "leaderboard",
           "fas fa-calendar-alt",
         )
       : React.null}
    {course |> Course.review
       ? courseLink(courseId, "Review", "review", "fas fa-check-square")
       : React.null}
    {course |> Course.review
       ? courseLink(courseId, "Students", "students", "fas fa-user-friends")
       : React.null}
    {communityLinks(course |> Course.linkedCommunities, communities)}
  </div>;
};

let coursesSection = (courses, communities, currentSchoolAdmin) => {
  <div className="w-full max-w-4xl mx-auto">
    <div className="flex flex-wrap flex-1 lg:-mx-5">
      {courses
       |> Array.map(course =>
            <div
              key={course |> Course.id}
              ariaLabel={course |> Course.name}
              className="w-full px-3 lg:px-5 md:w-1/2">
              <div
                key={course |> Course.id}
                className="flex overflow-hidden shadow bg-white rounded-lg flex flex-col justify-between h-full">
                <div>
                  <div className="relative">
                    <div className="relative pb-1/2 bg-gray-800">
                      {switch (course |> Course.thumbnailUrl) {
                       | Some(url) =>
                         <img
                           className="absolute h-full w-full object-cover"
                           src=url
                         />
                       | None =>
                         <div
                           className="user-home-course__cover absolute h-full w-full svg-bg-pattern-1"
                         />
                       }}
                    </div>
                    <div
                      className="user-home-course__title-container absolute w-full flex items-center h-16 bottom-0 z-50"
                      key={course |> Course.id}>
                      <h4
                        className="user-home-course__title text-white font-semibold leading-tight pl-6 pr-4">
                        {course |> Course.name |> str}
                      </h4>
                    </div>
                  </div>
                  <div
                    className="user-home-course__description text-sm px-6 pt-4 w-full leading-relaxed">
                    {course |> Course.description |> str}
                  </div>
                  {if (course |> Course.exited) {
                     <div className="text-sm p-4 bg-red-100 rounded">
                       {"Your student profile for this course is locked, and cannot be updated."
                        |> str}
                     </div>;
                   } else {
                     <div> {courseLinks(course, communities)} </div>;
                   }}
                </div>
                <div> {callToAction(course, currentSchoolAdmin)} </div>
              </div>
            </div>
          )
       |> React.array}
    </div>
  </div>;
};

let communitiesSection = communities => {
  <div className="w-full max-w-4xl mx-auto">
    <div className="flex flex-wrap flex-1 lg:-mx-5">
      {communities
       |> Array.map(community =>
            <div
              key={community |> Community.id}
              className="flex w-full px-3 lg:px-5 md:w-1/2">
              <a
                className="w-full h-full shadow rounded-lg hover:shadow-lg"
                href={"communities/" ++ (community |> Community.id)}>
                <div
                  className="user-home-community__cover flex w-full bg-gray-600 h-32 svg-bg-pattern-5 items-center justify-center p-4 shadow rounded-t-lg"
                />
                <div
                  className="w-full flex justify-between items-center flex-wrap px-4 pt-2 pb-4">
                  <h4 className="font-bold text-sm pt-2 leading-tight">
                    {community |> Community.name |> str}
                  </h4>
                  <div className="btn btn-small btn-primary-ghost mt-2">
                    {"Visit Community" |> str}
                  </div>
                </div>
              </a>
            </div>
          )
       |> React.array}
    </div>
  </div>;
};

[@react.component]
let make =
    (
      ~currentSchoolAdmin,
      ~courses,
      ~communities,
      ~showUserEdit,
      ~userName,
      ~userTitle,
      ~avatarUrl,
    ) => {
  let (view, setView) = React.useState(() => ShowCourses);
  <div className="bg-gray-100">
    <div className="bg-white">
      {headerSectiom(userName, userTitle, avatarUrl, showUserEdit)}
      {navSection(view, setView)}
    </div>
    <div className="py-8">
      {switch (view) {
       | ShowCourses =>
         coursesSection(courses, communities, currentSchoolAdmin)
       | ShowCommunities => communitiesSection(communities)
       }}
    </div>
  </div>;
};
