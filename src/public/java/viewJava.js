function getUsernameFromCookie()
{
    const cookies = document.cookie.split('; ');
    for (const cookie of cookies)
    {
        const [name, value] = cookie.split('=');
        if (name === 'user')
        {
            return decodeURIComponent(value);
        }
    }
    return 'You';
}

function escapeSingleQuotes(str) {
    return typeof str === 'string' ? str.replace(/'/g, "\\'") : '';
}

function escapeHTML(str) {
    return str.replace(/&/g, '&amp;')
        .replace(/"/g, '&quot;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;');
}

function startPostEdit(forumId, originalPost, originalDescription) {
    try {
        const postDiv = document.getElementById(`forum-${forumId}`);
        postDiv.innerHTML = `
            <form onsubmit="return submitPostEdit(event, ${forumId})">
                <input type="text" id="edit-input-${forumId}-post" value="${escapeHTML(originalPost)}">
                <input type="text" id="edit-input-${forumId}-description" value="${escapeHTML(originalDescription)}">
                <input type="submit" value="Save" class="save">
            </form>
        `;
        return false;
    } catch (err) {
        console.error("Error in startPostEdit:", err);
        alert("Error: " + err.message);
    }
}

function submitPostEdit(event, forumId) {
    event.preventDefault();

    const postInput = document.getElementById(`edit-input-${forumId}-post`);
    const updatePost = postInput.value;
    const descriptionInput = document.getElementById(`edit-input-${forumId}-description`);
    const updatedDescription = descriptionInput.value;

    try {
        fetch(`/forum/${forumId}/edit`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: `updatedPost=${encodeURIComponent(updatePost)}&updatedDescription=${encodeURIComponent(updatedDescription)}`
        })
            .then(res => {
                if (res.ok) {
                    location.reload();
                } else {
                    alert("Failed to update post.");
                }
            });

        return false;
    } catch (err) {
        console.error("Error in submitPostEdit:", err);
        alert("Error: " + err.message);
        return true;
    }
}

function startCommentEdit(commentId, originalComment) {
    try {
        const commentDiv = document.getElementById(`comment-${commentId}`);
        commentDiv.innerHTML =
            `
                <form onsubmit="return submitCommentEdit(event, ${commentId})">
                    <input type="text" id="edit-input-${commentId}" value="${originalComment}" required>
                    <input type="submit" value="Save" class="save">
                    <button type="button" onclick="cancelCommentEdit(event, ${commentId}, '${escapeSingleQuotes(originalComment)}')" class="cancel">Cancel</button>
                </form>
            `;
        return false;
    } catch (err) {
        console.error("Error in startCommentEdit:", err);
        alert("Error: " + err.message);
    }
}

function submitCommentEdit(event, commentId) {
    event.preventDefault();

    const input = document.getElementById(`edit-input-${commentId}`);
    const updatedComment = input.value;

    const commentDiv = document.getElementById(`comment-${commentId}`);
    const createdAt = commentDiv.dataset.createdAt;
    try {
        fetch(`/comment/${commentId}/edit`,
            {
                method: 'POST',
                headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                body: `updatedComment=${encodeURIComponent(updatedComment)}`
            })
        .then(res => {
            if (res.ok) {
                const originalUsername = commentDiv.dataset.username;
                commentDiv.innerHTML =
                    `
                    <p><strong>${originalUsername}</strong>: ${updatedComment}</p>
                    <small>${createdAt}</small>
                    <div style="display: flex; gap: 0.5em; margin-top: 0.5em;">
                        <form onsubmit="return startCommentEdit(${commentId}, '${escapeSingleQuotes(updatedComment)}')">
                            <input type="submit" value="Edit" class="edit-button">
                        </form>
                        <form onsubmit="return deleteComment(event, ${commentId})">
                            <input type="submit" value="Delete" class="edit-button">
                        </form>
                    </div>
                `;
            }
            else {
                alert("Failed to update comment.");
            }
        });

        return false;
    } catch (err) {
        console.error("Error in submitCommentEdit:", err);
        alert("Error: " + err.message);
        return true;
    }
}

function cancelCommentEdit(event, commentId, originalComment) {
    event.preventDefault();

    const commentDiv = document.getElementById(`comment-${commentId}`);
    const username = getUsernameFromCookie();

    commentDiv.innerHTML =
    `
        <p><strong>${username}</strong>: ${originalComment}</p>
        <small>just now</small>
        <div style="display: flex; gap: 0.5em; margin-top: 0.5em;">
            <form onsubmit="return startCommentEdit(${commentId}, '${escapeSingleQuotes(originalComment)}')">
                <input type="submit" value="Edit" class="edit-button">
            </form>
            <form onsubmit="return deleteComment(event, ${commentId})">
                <input type="submit" value="Delete" class="edit-button">
            </form>
        </div>
    `;
    return false;
}

function addComment(event, forumId) {
    event.preventDefault();
    const input = document.getElementById("new-comment-input");
    const comment = input.value;

    fetch(`/forum/${forumId}`,
    {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `comment=${encodeURIComponent(comment)}`
    })
    .then(res => {
        if (res.ok) {
            location.reload();
        }
        else {
            alert("Failed to post comment.");
        }
    });

    return false;
}

function deleteComment(event, commentId) {
    event.preventDefault();

    fetch(`/delete/${commentId}`,
    {
        method: 'POST'
    })
    .then(res => {
        if (res.ok) {
            location.reload();
        }
        else {
            alert("Failed to delete comment.");
        }
    });

    return false;
}